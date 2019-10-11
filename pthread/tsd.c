#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct tsd_tag {
    pthread_t thread_id;
    char *string;
} tsd_t;

pthread_key_t tsd_key;
pthread_once_t key_once = {PTHREAD_ONCE_INIT};

void once_routine(void)
{
    int status;

    printf("initializing key\n");

    status = pthread_key_create(&tsd_key, NULL);
    if (status != 0) {
        fprintf(stderr, "%d Create key\n", status);
    }
}


void *thread_routine(void *arg)
{
    tsd_t *value;
    int status;

    status = pthread_once(&key_once, once_routine);
    if (status != 0) {
        fprintf(stderr, "%d Once init\n", status);
    }

    value = (tsd_t *) malloc(sizeof (tsd_t));
    if (value == NULL) {
        fprintf(stderr, "Allocate key value\n");
    }

    status = pthread_setspecific(tsd_key, value);
    if (status != 0) {
        fprintf(stderr, "Set tsd\n");
    }
    printf("%lu %s set tsd value %p\n", pthread_self(), (char *) arg, value);
    
    value->thread_id = pthread_self();
    value->string = (char *) arg;

    value = (tsd_t *) pthread_getspecific(tsd_key);
    printf("%lu %s starting...\n", pthread_self(), value->string);

    sleep(10);

    value = (tsd_t *) pthread_getspecific(tsd_key);
    printf("%lu %s done...\n", pthread_self(), value->string);

    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    int status;

    status = pthread_create(&thread1, NULL, thread_routine, "thread1");
    if (status != 0) {
        fprintf(stderr, "create thread 1\n");
    }

    status = pthread_create(&thread2, NULL, thread_routine, "thread2");
    if (status != 0) {
        fprintf(stderr, "create thread 2\n");
    }

    pthread_exit(0);

    return 0;
}
