#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>

void sig_handler(int signo);

static jmp_buf loc_info;

int main()
{
  int  i = 0, rtn;
  char *test = NULL;

  signal(SIGSEGV, SIG_IGN);

  if (setjmp(loc_info) == 0) {
    printf("test1 : %s\n", test[1]);
  }
  else {
    printf("test2\n");
  }


  return 0;
}

void sig_handler(int signo)
{
  printf("SIGSEGV create %d\n", signo);
  _exit(0);
  //longjmp(loc_info, 1);
}
