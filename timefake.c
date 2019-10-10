/* 
 * 1. compile
 * in Spark C compiler
 * cc -G -o libtimefake.so libtimefake.c 
 * in FreeBSD or Linux (GNU C compiler)
 * cc -shared -o libtimefake.so libtimefake.c
 *
 * 2. setup
 * chmod a+x /home/user/lib/libtimefake.so
 * in Bourne-shell
 * LD_PRELOAD=/home/user/lib/libtimefake.so
 * export LD_PRELOAD
 * hash -r
 * in Csh
 * setenv LD_PRELOAD /home/user/lib/libtimefake.so
 * rehash
 *
 * 3. use
 * #!/bin/sh
 * LD_PRELOAD=/home/user/...
 * export ...
 * stubborn "$@"
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define YEAR_OFFSET 1900
#define MONTH_OFFSET 1

#define year (2018 - YEAR_OFFSET)
#define month (3 - MONTH_OFFSET)
#define day 17

time_t time(time_t *loc)
{
    struct tm thetime;
    time_t faketime;
  
    thetime.tm_sec = 0;
    thetime.tm_min = 0;
    thetime.tm_hour = 0;
    thetime.tm_mday = day;
    thetime.tm_mon = month;
    thetime.tm_year = year;

    faketime = mktime(&thetime);

    if (loc) 
        *loc = faketime;
    return faketime;
}
