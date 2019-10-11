#include <stdlib.h>
#include <string.h>


int next[1024];

void initnext(char *p)
{
    int i, j, M = strlen(p);

    next[0] = -1;

    for (i = 0, j = -1; i < M; i++, j++, next[i] = j)
	while ((j >= 0) && (p[i] != p[j]))
	    j = next[j];
}


int kmpsearch(char *p, char *a)
{
    int i, j, M, N;

    if (p == NULL || a == NULL)
	return -1;
    else
	M = strlen(p), N = strlen(a);

    initnext(p);

    for (i = 0, j = 0; j < M && i < N; i++, j++)
	while ((j >= 0) && (a[i] != p[j]))
	    j = next[j];
    if (j == M)
	return i - M;
    else
	return i;
}


int main()
{
    kmpsearch("haystack", "st");
}
