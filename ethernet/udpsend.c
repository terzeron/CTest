#include <stdio.h>
#include <unistd.h> // write
#include <err.h> // errx
#include <errno.h> // errno
#include <string.h> // bzero
#include <sys/types.h> // u_int8_t 
#include <sys/socket.h> // socket, connect
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_pton

#define SERV_ADDR "10.0.0.255"
#define SERV_PORT 137 // netbios-ssn
#define CLI_ADDR "10.0.0.23"
#define CLI_PORT 137 // netbios-ssn


int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    char msg[4096];
    int ch;
    int len = 0;

    while ((ch = getchar()) != EOF) {
        msg[len++] = ch;
    }
    printf("len = %d\n", len);

    // socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        errx(1, "can't open socket descriptor");
    }
    
    // server 林家
    bzero(&serv_addr, sizeof (struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_ADDR, &serv_addr.sin_addr);

    // client 林家
    bzero(&cli_addr, sizeof (struct sockaddr_in));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(0);
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if (bind(sockfd, (struct sockaddr *) &cli_addr, 
             sizeof (struct sockaddr_in)) < 0) {
        errx(1, "can't bind");
    }
    
    if (sendto(sockfd, msg, len, 0, (struct sockaddr *) &serv_addr, 
               sizeof (struct sockaddr_in)) 
        < 0) {
        errx(1, "can't send message to server, %s", strerror(errno));
    }
	
    shutdown(sockfd, SHUT_WR);
    close(sockfd);

    return 0;
}






