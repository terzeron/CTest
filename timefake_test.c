#include <stdio.h>
#include <time.h>


int main()
{
    time_t t;
    struct tm* timeinfo;
    char buf[20];
    
    time(&t);
    timeinfo = localtime(&t);
    strftime(buf, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("%s\n", buf);

    return 0;
}
