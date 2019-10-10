#include <stdio.h>
#include <time.h>

int main() 
{
    struct tm time;
    printf("\n\n\n*** Present Time *** ");
    printf("\nHour : %d", time.tm_hour);
    printf("\nMinute : %d", time.tm_min);
    printf("\nSecond : %d", time.tm_sec);
    return 0;
}

