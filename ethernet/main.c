#include <stdio.h> // printf
#include <fcntl.h> // open
#include <stdlib.h> // calloc, free
#include <unistd.h> // close
#include <string.h> // string
#include <assert.h> // assert
#include <err.h> // errx
#include <errno.h> // errno
#include <sys/ioctl.h> // ioctl
#include <sys/types.h> // size_t
#include <sys/socket.h> // sockaddr
#include <net/if.h> // ifru_addr, ifreq
#include <pcap/bpf.h> // BIOCSETIF, BIOCSHDRCMPLT
#include <net/ethernet.h> // ether_header 
#include "arpproxy.h" 

#define DEVICE_NAME_LEN 16
#define DEVICE_NAME "dc1"

struct ether_handle *ether_open(char *device);
void ether_close(struct ether_handle *e);
size_t ether_send(struct ether_handle *e, void *buf, size_t len);

struct ether_handle {
    int fd;
    char device[DEVICE_NAME_LEN];
};


int main()
{
    struct ether_handle *e;
    size_t ret;
    size_t length = 0;
    static unsigned char buf[4096];
    int ch;
    
    while ((ch = getchar()) != EOF) {
        buf[length++] = ch;
    }
    printf("length = %ld\n", length);

    e = ether_open(DEVICE_NAME);
    ret = ether_send(e, buf, length + 14);
    if (ret < 0) {
        errx(1, "can't send ethernet frame");
    }
    ether_close(e);

    return 0;
}


struct ether_handle *ether_open(char *device)
{
    int i;
    struct ether_handle *e;
    char file[32];
    struct ifreq ifr;
    
    e = (struct ether_handle *) calloc(1, sizeof(struct ether_handle));
    if (e == NULL) {
        errx(1, "can't allocate memory");
    }
    
    for (i = 0; i < 32; i++) {
        snprintf(file, 32, "/dev/bpf%d", i);
        e->fd = open(file, O_WRONLY);
        if (e->fd != -1 || errno != EBUSY) {
            break;
        }
    }
    if (e->fd < 0) {
        ether_close(e);
        errx(1, "can't open bpf file");
    }
    
    memset(&ifr, 0, sizeof (struct ifreq));
    strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));

    if (ioctl(e->fd, BIOCSETIF, (char *) &ifr) < 0) {
        ether_close(e);
        errx(1, "can't do ioctl for setting interface for bpf");
    }

    i = 1; 
    if (ioctl(e->fd, BIOCSHDRCMPLT, &i) < 0) {
        ether_close(e);
        errx(1, "can't do ioctl for setting header complete");
    }
    
    strncpy(e->device, device, DEVICE_NAME_LEN);

    return e;
}


void ether_close(struct ether_handle *e) 
{
    assert(e != NULL);
    
    if (e->fd > 0) {
        close(e->fd);
    }
    free(e);

    return;
}


size_t ether_send(struct ether_handle *e, void *buf, size_t len)
{
    return ((size_t) write(e->fd, buf, len));
}

















