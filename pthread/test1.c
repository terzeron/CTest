#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_cond_t      cond;

int main(int argc, char **argv)
{
    int                   rc=0;
    pthread_condattr_t    attr;

    printf("Entering testcase\n");

    printf("Create a default condition attribute\n");
    rc = pthread_condattr_init(&attr);
    checkResults("pthread_condattr_init\n", rc);

    printf("Create the condition using the condition attributes object\n");
    rc = pthread_cond_init(&cond, &attr);
    checkResults("pthread_cond_init()\n", rc);

    printf("- At this point, the condition with its default attributes\n");
    printf("- Can be used from any threads that want to use it\n");

    printf("Destroy cond attribute\n");
    rc = pthread_condattr_destroy(&attr);
    checkResults("pthread_condattr_destroy()\n", rc);
  
    printf("Destroy condition\n");
    rc = pthread_cond_destroy(&cond);
    checkResults("pthread_cond_destroy()\n", rc);
  
    printf("Main completed\n");
    return 0;
}


