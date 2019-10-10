#include <stdio.h>

extern int seminit(void);
extern int semfinal(void);
extern int semlock(void);
extern int semunlock(void);

int main()
{
	int i;

	seminit();
	semlock();
	for (i = 0; i < 1000000000; i++) {
		;
	}
	semunlock();
	return 0;
}
