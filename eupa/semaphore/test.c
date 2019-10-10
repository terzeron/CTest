#include <stdio.h>

extern int seminit(void);
extern int semfinal(void);
extern int semlock(void);
extern int semunlock(void);

int main()
{
	int i;
	int count = 0;

	semlock();
	for (i = 0; i < 60; i++) {
		printf("#");
	}
	printf("\n");
	for (i = 0; i < 100000000; i++) {
		count += i;
	}
	for (i = 0; i < 60; i++) {
		printf("#");
	}
	printf("\n");
	semunlock();
	return 0;
}
