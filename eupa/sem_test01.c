#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


int get_sem_value(sem_t *sem)
{
	int value;
	if (sem_getvalue(sem, &value)) {
		perror("cant' get the value of semaphore");
		return -1;
	}
	return value;
}
	
void print_and_sleep(int count)
{
	int i = 0;
	for (i = 0; i < count; i++) {
		putchar('+');
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
}

int main()
{
	int count = 0;
	char sem_name[] = "/mysem";
	sem_unlink(sem_name);
	sem_t *mysem = sem_open(sem_name, O_CREAT, 0644, 1);
	if (mysem == SEM_FAILED) {
		perror("can't create semaphore");
		return -1;
	}

	srand(time(NULL));
	printf("start...\n");
	while (1) {
		count = rand() % 10;
		sem_wait(mysem);
		// critical section
		printf("sleeping %d second(s)...\n", count);
		print_and_sleep(count);
		sem_post(mysem);
		sleep(rand() % 2);
	}
	printf("end...\n");
	sem_close(mysem);
	return 0;
}

