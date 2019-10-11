#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct _global_desc_t {
    pthread_cond_t var1;
    pthread_cond_t var2;
    long status_flag;
    int error_num;
    //struct _tuple_msg_t *msg_ptr;
    pthread_mutex_t mutex;
    int cond_hold;
} global_desc_t;

#define SHM_KEY 23526
#define SHM_SIZE (sizeof (global_desc_t))
#define SHM_MODE (00666 | IPC_CREAT)


#define DEBUG
#ifdef DEBUG
#define err_print(_fmt, _args...) fprintf(stderr, "%s:%d: "_fmt, \
__FILE__, __LINE__, ##_args)
#else // DEBUG
#define err_print(_fmt, _args...)
#endif // DEBUG



char *shared_memory_init()
{
    int shmid;
    void *shmptr = NULL;

    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
        printf("Already exists shared memory with respect to the key\n");
        if ((shmid = shmget(SHM_KEY, SHM_SIZE, 0666)) < 0) {
            err_print("shmget: %s\n", strerror(errno));
        }
    }

    printf("shmid=%d(%X)\n", shmid, shmid);
    
    if ((shmptr = shmat(shmid, (const void *) NULL, 0)) == (void *) -1) {
        err_print("shmat: %s\n", strerror(errno));
        return NULL;
    }
    
    printf("shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE);

#if 0
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        err_print("shmctl: %s\n", strerror(errno));
    }
#endif
    
    return shmptr;
}


char *memory_map()
{
    void *shmptr = NULL;
    int fd;
    struct stat st;
    int map_size = SHM_SIZE;
    char filename[] = "testmapfile";

    fd = open(filename, O_RDWR|O_CREAT, 0666);
    if (fd < 0) {
        err_print("open: %s\n", strerror(errno));
        return NULL;
    }
    if (stat(filename, &st) < 0) {
        err_print("stat: %s\n", strerror(errno));
        return NULL;
    }
    
    if (st.st_size < map_size) {
        char zero_string[] = "0";

        if (lseek(fd, map_size - 1, SEEK_SET) < 0) {
            err_print("lseek: %s\n", strerror(errno));
            return NULL;
        }

        if (write(fd, zero_string, 1) < 0) {
            err_print("write: %s\n", strerror(errno));
            return NULL;
        }
    } else {
        map_size = st.st_size;
    }
    
    shmptr = mmap(NULL, map_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmptr == MAP_FAILED) {
        err_print("mmap: %s\n", strerror(errno));
        return NULL;
    }

    return shmptr;
}


int main(int argc, char *argv[])
{
    global_desc_t *desc;
    char *shmptr;
    char c;

    // 테스트용 공유 메모리 
#if 1
    // SVR4 IPC shared memory
    shmptr = shared_memory_init();
#else
    // POSIX memory mapping
    shmptr = memory_map();
#endif

    desc = (global_desc_t *) shmptr;
    printf("desc=%p\n", desc);
    desc->cond_hold = 1;

#if 0
    if (mutexattr_init(&desc->mattr) < 0) {
        err_print("mutexattr_init: %s\n", strerror(errno));
        return -1;
    }
#endif
    pthread_mutexattr_t mutexattr;
    if (pthread_mutex_init(&desc->mutex, &mutexattr) < 0) {
        err_print("mutex_init: %s\n", strerror(errno));
        return -1;
    }
#if 0
    if (mutexattr_setpshared(&desc->mattr, PROCESS_SHARED) 
        < 0) {
        err_print("condattr_setpshared: %s\n", strerror(errno));
        return -1;
    }
#endif

#if 0
    if (condattr_init(&desc->cattr) < 0) {
        err_print("condattr_init: %s\n", strerror(errno));
        return -1;
    }
    if (condattr_setpshared(&desc->cattr, PROCESS_SHARED) 
        < 0) {
        err_print("condattr_setpshared: %s\n", strerror(errno));
        return -1;
    }
#endif
    pthread_condattr_t condattr;
    if (pthread_cond_init(&desc->var1, &condattr) < 0) {
        err_print("cond_init: %s\n", strerror(errno));
        return -1;
    }

    c = getopt(argc, argv, "12");
    switch (c) {
    case '1':
#if 1
        printf("mutex_lock()\n");
        pthread_mutex_lock(&desc->mutex);
#endif

        desc->cond_hold = 0;
#if 1
        printf("cond_signal() desc=%p desc->error_num=%d(%p)\n", desc, desc->error_num, &desc->error_num);
        pthread_cond_signal(&desc->var1);
#else
        printf("cond_broadcast() desc=%p desc->error_num=%d(%p)\n", desc, desc->error_num, &desc->error_num);
        pthread_cond_broadcast(&desc->var1);
#endif

#if 1
        printf("mutex_unlock()\n");
        pthread_mutex_unlock(&desc->mutex);
#endif
        break;
    case '2':
#if 1
        printf("mutex_lock()\n");
        pthread_mutex_lock(&desc->mutex);
#endif
        desc->error_num = 20;
        printf("cond_wait() desc=%p desc->error_num=%d(%p)\n", desc, desc->error_num, &desc->error_num);
        //while (desc->cond_hold) {
        pthread_cond_wait(&desc->var1, &desc->mutex);
            //}
#if 1
        printf("mutex_unlock()\n");
        pthread_mutex_unlock(&desc->mutex);
#endif
        break;
    default:
        break;
    }

    return 0;
}
