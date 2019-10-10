#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define PROGRAM "timechecker"

int main(void)
{
  int result = 0;
  time_t present, next; 
  struct tm thetime;
 
  present = time(NULL);

  thetime.tm_sec = 59;
  thetime.tm_min = 59;
  thetime.tm_hour = 11;
  thetime.tm_mday = 31;
  thetime.tm_mon = 12;
  thetime.tm_year = 99;
  next = mktime(&thetime);
   
  printf("Present:%d Next:%d\n", present, next);

  result = stime(&next);
  if (result == -1 && errno == EPERM) {
    fprintf(stderr, "%s: You aren't a superuser.\n", PROGRAM);
    return -1;
  }
  sleep(2);
  system("date");
   
  result = stime(&present);
  if (result == -1 && errno == EPERM) {
    fprintf(stderr, "%s: You aren't a superuser.\n", PROGRAM);
    return -1;
  }
  return 0;
}
