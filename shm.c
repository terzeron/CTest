#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY1 0x345678
#define KEY2 0x885678
#define KEY3 0x885679

int main(void)
{
	int shmid1 = 0;
    //int shmid2 = 0;
    //int shmid3 = 0;
    char *shmptr1 = NULL;
    //char *shmptr2 = NULL;
	//char *shmptr3 = NULL;	
    //int test;
    
	shmid1 = shmget(KEY2, 10000, IPC_CREAT|SHM_R|SHM_W);
	if (shmid1 < 0) {
		fprintf(stderr, "shmget error\n");
		return -1;
	}
    printf("shmid1=%d\n", shmid1);

    shmptr1 = shmat(shmid1, 0, 0);
    if (shmptr1 == (void *) -1) {
        fprintf(stderr, "shmat error\n");
        return -1;
    }
    printf("shmptr1=%p\n", shmptr1);

	return 0;
}


