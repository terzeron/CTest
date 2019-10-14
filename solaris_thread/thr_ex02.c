#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <thread.h>

#define MAX_THREADS 10
#define TIMEOUT 3

thread_t tid[MAX_THREADS];
mutex_t mp;
cond_t cv;
int id = 0;

void *thread_function(void *data) 
{
    struct timespec to;
    int ret;

    mutex_lock(&mp);
    to.tv_sec = time(NULL) + thr_self();
    to.tv_nsec = 0;
    while (id > 5 && id < 8) {
        ret = cond_timedwait(&cv, &mp, &to);
        if (ret == ETIME) {
            break;
        }
    }
    if (thr_self() == 6) {
        cond_broadcast(&cv);
    }
    id = thr_self();
    printf("thread %d, id=%d\n", thr_self(), id);
    mutex_unlock(&mp);

    return NULL;
}


int main()
{
    int i;
    int count = 10;

    srand(time(NULL));

    if (cond_init(&cv, USYNC_THREAD, NULL) < 0) {
        fprintf(stderr, "can't initialize a condition variable, errno=%d %s\n", 
                errno, strerror(errno));
        exit(-1);
    }

    for (i = 0; i < count; i++) {
        if (thr_create(NULL, 0, thread_function, &i, 0, &tid[i]) < 0) {
            fprintf(stderr, "can't create a thread %d, errno=%d %s\n", 
                    i, errno, strerror(errno));
            exit(-1);
        }
    }
    
    for (i = 0; i < count; i++) {
        thr_join(tid[i], NULL, NULL);
    }

    return 0;
}


