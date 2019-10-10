#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull_t;
ull_t *counts;
ull_t ul;

void store_count(ull_t i, ull_t count)
{
    if (i <= ul) {
	counts[i] = count;
	//printf("#[%d]=%d\n", i, count);
    }
}


ull_t count_iteration(ull_t i)
{
    ull_t ret;
    ull_t index = i;

    //printf("i=%llu\n", i);
    if (i == 1) {
	ret = 1;
    } else if (i % 2 == 0) {
	if (i / 2 <= ul && counts[i / 2] > 0) {
	    //printf("[%llu]=%llu\n", i / 2, counts[i / 2]);
	    ret = counts[i / 2] + 1;
	} else {
	    ret = count_iteration(i / 2) + 1;
	}
    } else {
	if (i * 3 + 1 <= ul && counts[i * 3 + 1] > 0) {
	    //printf("[%llu]=%llu\n", i * 3 + 1, counts[i * 3 + 1]);
	    ret = counts[i * 3 + 1] + 1;
	} else {
	    ret = count_iteration(i * 3 + 1) + 1;
	}
    }
    
    //printf("[%llu]<<=%llu\n", index, ret);
    if (index <= ul && counts[index] > 0) 
	store_count(index, ret);
    return ret;
}


int main(void)
{
    ull_t i;
    ull_t j;
    ull_t k;
    ull_t count;
    ull_t max;
    ull_t min;
    ull_t max_len;

    while (scanf("%llu%llu", &i, &j) == 2) {
	max = (i > j ? i : j);
	min = (i > j ? j : i);
	max_len = 1;
	ul = max;
	counts = (ull_t *) malloc(sizeof (ull_t) * (max + 1));
	memset(counts, 0, sizeof (ull_t) * (max + 1));
    
	for (k = min; k <= max; ++k) {
	    //printf("k=%llu\n", k);
	    count = count_iteration(k);
	    max_len = (max_len < count ? count : max_len);
	    //printf("k=%llu: count=%llu\n\n", k, count);
	}
	printf("%llu %llu %llu\n", i, j, max_len);
    }
    return 0;
}
