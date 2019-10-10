#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int shmid1 = 0;
char *shmptr1 = NULL;
int flag = IPC_CREAT | SHM_R | SHM_W;

int main(int argc, char *argv[])
{
    shm_init();
	register_func();

	return 0;
}

int shm_init()
{
    shmid1 = shmget(KEY1, 10000, flag);
    if (shmid1 < 0) {
        fprintf(stderr, "shmget error, %s\n", strerror(errno));
        return -1;
    }
    printf("shmid1=%d\n", shmid1);
    
	if ((shmptr1 = shmat(shmid1, 0, 0)) < 0) {
		fprintf(stderr, "shmat error, %s\n", strerror(errno));
	}
	printf("shmptr1=%p\n", shmptr1);

    return 0;
}


