#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define USE_DYNAMIC_PROG

#ifdef USE_DYNAMIC_PROG
long long *counts;
long long ul;

void store_count(long long i, long long count)
{
    if (i <= ul) {
	counts[i] = count;
	//printf("#[%d]=%d\n", i, count);
    }
}
#endif				// USE_DYNAMIC_PROG


long long count_iteration(long long i)
{
    long long count = 0;
#ifdef USE_DYNAMIC_PROG
    long long seed = i;
#endif				// USE_DYNAMIC_PROG

    //printf("\t");
    while (++count) {
#ifdef USE_DYNAMIC_PROG
	if (i <= ul && counts[i] > 0) {
	    //printf("[%d]\n", i);
	    store_count(seed, count + counts[i] - 1);
	    return count + counts[i] - 1;
	}
#endif				// USE_DYNAMIC_PROG
	//printf("%d ", i);
	if (i == 1) {
	    break;
	} else if (i % 2 == 0) {
	    i = i / 2;
	} else {
	    i = i * 3 + 1;
	}
    }
    //printf("\n");
#ifdef USE_DYNAMIC_PROG
    store_count(seed, count);
#endif				// USE_DYNAMIC_PROG
    return count;
}


int main(void)
{
    long long i;
    long long j;
    long long k;
    long long count;
    long long max;
    long long min;
    long long max_len;

    while (scanf("%lld%lld", &i, &j) == 2) {
	max = (i > j ? i : j);
	min = (i > j ? j : i);
	max_len = 1;
#ifdef USE_DYNAMIC_PROG
	ul = max;
	counts = (long long *) malloc(sizeof(long long) * (max + 1));
	memset(counts, 0, sizeof(long long) * (max + 1));
#endif				// USE_DYNAMIC_PROG

	for (k = min; k <= max; ++k) {
	    count = count_iteration(k);
	    max_len = (max_len < count ? count : max_len);
	    //printf("%d: count=%d\n", k, count);
	}
	printf("%lld %lld %lld\n", i, j, max_len);
    }
    return 0;
}
