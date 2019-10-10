#include <stdio.h>
#include <stdlib.h>


int isleap(int year);
int totaldays(int year, int month, int day, int leap);

int main(void)
{
    int year1, month1, day1;
    int year2, month2, day2;
    int leap1, leap2;
    int totald1, totald2;
    int diff = 0;
    char dayofweek[7][7] = {"Sun", "Mon", "Tue", "Wednes", "Thurs", "Fri", "Satur"};
  
    printf("Input two days(year month day)\n");
    printf("First day: ");
    scanf("%d%d%d", &year1, &month1, &day1);
    printf("Second day: ");
    scanf("%d%d%d", &year2, &month2, &day2);


    leap1 = isleap(year1);
    leap2 = isleap(year2);
#ifdef DEBUG
    printf("leap %d %d\n", leap1, leap2);
#endif
    totald1 = totaldays(year1, month1, day1, leap1);
    totald2 = totaldays(year2, month2, day2, leap2);
#ifdef DEBUG
    printf("total %d %d\n", totald1, totald2);
#endif
    printf("%d %d %d is %sday.\n", year1, month1, day1, dayofweek[totald1 % 7]);
    printf("%d %d %d is %sday.\n", year2, month2, day2, dayofweek[totald2 % 7]);
    diff = totald1 - totald2;
    if (diff < 0)
        diff = diff * (-1);
    printf("Two days differ %d day(s).\n", diff);
  
    return 0;
}
 
     
int isleap(int year)
{
    if ((year % 400) == 0)
        return 1;
    else if ((year % 100) == 0)
        return 0;
    else if ((year % 4) == 0)
        return 1;
    else 
        return 0;
}

int totaldays(int year, int month, int day, int leap)
{
    int total = 0;
    int totalleap = 0;
    int offset = 1;
    int i;
    int daysofmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
    if (year < 1900) {
        fprintf(stderr, "Error : Invalid year input\n");
        exit(-1);
    } 
  
    if (month < 1 || month > 12) {
        fprintf(stderr, "Error : Invalid month input\n");
        exit(-1);
    } 
  
    if (leap == 0 && month == 2 && day == 29) {
        fprintf(stderr, "Error : Invalid leap day input\n");
        exit(-1);
    }
  
    if ((day > daysofmonth[month-1]) && 
        !(leap == 1 && month == 2 && day == 29)) {
        fprintf(stderr, "Error : Invalid day input\n");
        exit(-1);
    } 
  
    totalleap = (year-1901) / 400 - (year-1901) / 100 + (year-1901) / 4;
#ifdef DEBUG
    printf("totalleap :%d\n", totalleap);
#endif

    total = (year - 1900) * 365;
    for (i = 0; i < month-1; i++) {
        if (i == 1) {
#ifdef DEBUG
            printf("add %dmonth %dday\n", i+1, daysofmonth[i]+leap);
#endif
            total += (daysofmonth[i]+leap);
        } else {
#ifdef DEBUG
            printf("add %dmonth %dday\n", i+1, daysofmonth[i]);
#endif
            total += daysofmonth[i];
        }
    }
    total += (day - 1);
  
    return total+offset+totalleap;
}
