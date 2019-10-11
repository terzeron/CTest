#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 0x345678

int x[1024];

int main(void)
{
	int shmid;
	char *shmptr = NULL;
    //int y[1024] = { 1, };
    //char *test = (char *) malloc(sizeof (char) * 1024);

	shmid = shmget(KEY, 10000, IPC_CREAT|SHM_R|SHM_W);
	if (shmid < 0) {
		fprintf(stderr, "shmget error\n");
		return -1;
	}
	shmptr = shmat(shmid, 0, 0);
	if (shmptr == (void *) -1) {
		fprintf(stderr, "shmat error\n");
		return -1;
	}
	printf("shmptr=%p\n", shmptr);

	getchar();	

	return 0;
}


