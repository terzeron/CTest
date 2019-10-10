#include <stdio.h>
#include <time.h>

int main()
{
    struct tm theday;
    time_t theday_time;
    time_t today;
    double diff;
    int year;
    int day;
    int hour;
    int minute;
    int second;
    
    theday.tm_sec = 0;
    theday.tm_min = 0;
    theday.tm_hour = 19;
    theday.tm_mday = 8;
    theday.tm_mon = 11;
    theday.tm_year = 100;
    theday.tm_wday = 5;
    //theday.tm_yday = ?;
    theday.tm_isdst = 0;
    theday_time = mktime(&theday);
    today = time(NULL);

    printf("%s", ctime(&theday_time));
    printf("%s", ctime(&today));

    diff = difftime(today, theday_time);
    
    year = (int) diff / (60 * 60 * 24 * 365);
    day = ((int) diff % (60 * 60 * 24 * 365)) / (60 * 60 * 24);
    hour = ((int) diff % (60 * 60 * 24)) / (60 * 60);
    minute = ((int) diff % (60 * 60)) / 60;
    second = (int) diff % 60;
    
    printf("%f seconds --> %d years, %d days, %d hours, %d minutes, %d seconds\n", diff, year, day, hour, minute, second);

    return 0;
}
