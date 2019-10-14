#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <thread.h>

#define _REENTRANT
void *thread_specific_data(void *data);
void *free_specific_data(void *value);
#define MAX_ARGC 20
thread_t tid[MAX_ARGC];
int num_threads;


int main(int argc, char *argv[])
{
    int i;

    num_threads = argc - 1;
    printf("num_threads=%d\n", num_threads);
    
    for (i = 0; i < num_threads; i++) {
        //printf("i=%d\n", i);
        if (thr_create(NULL, 0, thread_specific_data, argv[i+1], 0, &tid[i]) 
            < 0) {
            fprintf(stderr, "can't create a thread %d, errno=%d %s\n", 
                    i, errno, strerror(errno));
            exit(-1);
        }
    }

    for (i = 0; i < num_threads; i++) {
        thr_join(tid[i], NULL, NULL);
    }

    return 0;
}


void *thread_specific_data(void *private_data)
{
    static mutex_t keylock;
    static thread_key_t key;
    static int once_per_keyname = 0;
    void *tsd = NULL;

    printf("#thread_specific_data(%d)\n", thr_self());

    if (!once_per_keyname) {
        mutex_lock(&keylock);
        if (!once_per_keyname) {
            // thr_keycreate
            thr_keycreate(&key, free_specific_data(&key));
            once_per_keyname++;
        }
        mutex_unlock(&keylock);
    }
    // thr_getspecific
    thr_getspecific(key, &tsd);
    if (tsd == NULL) {
        tsd = malloc(strlen(private_data) + 1);
        strcpy(tsd, private_data);
        // thr_setspecific
        thr_setspecific(key, tsd);
        // thr_getspecific
        thr_getspecific(key, &tsd);
    }
     
    printf("tsd for %d = %s\n", thr_self(), (char *) tsd);
    thr_getspecific(key, &tsd);
    printf("tsd for %d remains %s\n", thr_self(), (char *) tsd);

    return tsd;
}


void *free_specific_data(void *value)
{
    free(value);
    return NULL;
}
