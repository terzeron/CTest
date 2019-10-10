#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "pv.h"

void handlesem(key_t skey);

int main()
{
    key_t semkey = 0x200;

    /*
      if (fork() == 0)
      handlesem(semkey);
      if (fork() == 0)
      handlesem(semkey);
      if (fork() == 0)
      handlesem(semkey); 
    */
    handlesem(semkey);
    return 0;
}

void handlesem(key_t skey)
{
    int mutex, pid = getpid();

    if ((mutex = init(skey)) < 0)
        exit(1);

    printf("\nprocess %d before critical section\n", pid);
    lock(mutex);
    printf("process %d in critical section\n", pid);

    /* in real life, do soemthing interesting */

    sleep(10);

    printf("process %d leaving critical section\n", pid);
    unlock(mutex);
    printf("process %d exiting\n", pid);

    exit(0);
}


