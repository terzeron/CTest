#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2

struct _data {
    int count;
    pthread_mutex_t *lock_var;
};

extern int errno;

void *add(void *arg)
{
    struct _data *temp = (struct _data *) arg;
    static int sum = 0;
    int i;
    char ret_char = '\n';

    struct timespec rqtp;
    struct timespec rmtp;

    rqtp.tv_sec = (time_t) 3;
    rqtp.tv_nsec = 0L;

    /*
      pthread_mutex_lock(temp->lock_var);
      for (i = 0; i < 100; i++) {
      printf("second thread\n");
      }
      pthread_mutex_unlock(temp->lock_var);
    */

    while (2) {
        pthread_mutex_lock(temp->lock_var);
        nanosleep(&rqtp, &rmtp);
        pthread_mutex_unlock(temp->lock_var);
        printf("second thread %d.%ld\n", (int) rmtp.tv_sec, rmtp.tv_nsec);
	
    }

    return NULL;
}

void *delete(void *arg)
{
    struct _data *temp = (struct _data *) arg;
    static int sum = 0;
    int i;
    char ret_char = '\n';

    struct timespec rqtp;
    struct timespec rmtp;

    rqtp.tv_sec = (time_t) 0;
    rqtp.tv_nsec = 100000000L;

    while (1) {
        pthread_mutex_lock(temp->lock_var);
        nanosleep(&rqtp, &rmtp);
        pthread_mutex_unlock(temp->lock_var);
        printf("first thread %d.%ld\n", (int) rmtp.tv_sec, rmtp.tv_nsec);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *thread;
    void *ret;
    int i;
    struct _data *data;
    pthread_mutex_t init_lock_var;
    
    thread = (pthread_t *) malloc(sizeof(pthread_t) * N);
    data = (struct _data *) malloc(sizeof(struct _data) * N);

    pthread_mutex_init(&init_lock_var, NULL);

    for (i = 0; i < N; i++) {
        data[i].count = i;
        data[i].lock_var = &init_lock_var;

        if (i == 0) {
            if (pthread_create(&thread[i], NULL, &delete, &data[i])) {
                fprintf(stderr, "%s: can't make thread\n", argv[0]);
                exit(-1);
            }
        } else {
            if (pthread_create(&thread[i], NULL, &add, &data[i])) {
                fprintf(stderr, "%s: can't make thread\n", argv[0]);
                exit(-1);
            }
        }
    }

    for (i = 0; i < N; i++) {
        if (pthread_join(thread[i], &ret)) {
            fprintf(stderr, "%s: can't join thread\n", argv[0]);
            exit(-1);
        }
    }
    
    return 0;
}





