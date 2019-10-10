#include <stdio.h>
#include <signal.h>

struct sigaction sa;
struct sigaction sa_old;

void myexit(int signum)
{
    printf("myexit()\n");
    //sigaction(SIGSEGV, &sa_old, NULL);
}


int main(void)
{
    sa.sa_handler = myexit;
    sa.sa_flags = 0;
    sigaction(SIGSEGV, &sa, &sa_old);
    
    int* p = NULL;
    printf("%d\n", *p);
    
    return 0;
}
