#include <stdio.h>

struct A {
    int a;
    int c;
};


int main(void)
{
    struct A a;
    struct A b;

    b.a = 1;
    b.c = 3;

    a = b;

    printf("%d %d\n", a.a, a.c);
    return 0;
}

