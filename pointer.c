#include <stdio.h>

int main()
{
    int a[] = {0, 1, 2, 3, 4};
    int *p[] = {a, a + 1, a + 2, a + 3, a + 4};
    int **pp = p;

    printf("&a=%p a=%p *a=%d a[0]=%d \n", &a, a ,*a, a[0]);
    printf("&p=%p p=%p *p=%p **p=%d p[0]=%p *p[0]=%d\n", &p, p, *p, **p, p[0], *p[0]);
    printf("&pp=%p pp=%p *pp=%p **pp=%d \n", &pp, pp, *pp, **pp);
    printf("&a[0]=%p", &a[0]);
}


