#include <stdio.h>
#include <string.h>
#include <sys/types.h>

struct flags {
    u_int16_t a:8;
    u_int8_t b:4;
    u_int8_t c:4;
    u_int8_t d:8;
} test_flag; // c(8) a(4) b(4) d(4)

int main()
{
    long long a;

    test_flag.a = 0x11;
    test_flag.b = 0x5;
    test_flag.c = 0x2;
    test_flag.d = 0x13;

    printf("sizeof testflag = %ld\n", sizeof test_flag);
    
    memcpy(&a, &test_flag, sizeof a);

    printf("a = 0x%llX, %lld\n", a, a); // d(8) b(4) a(4) c(8)

    return 0;
}
