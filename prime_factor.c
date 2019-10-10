#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	long long i;
	long long sq;
	long long number;

	if (argc != 2) {
		return 0;
	}
	
	sscanf(argv[1], "%lld", &number);
	sq = (long long) sqrt((double) number);
	printf("number=%lld sqrt(number)=%lld\n", number, sq);

	i = 2;
	while (i <= sq) {
		if (number % i == 0) {
			printf("mod=0 i=%lld\n", i);
			//printf("%lld\n", number/i);
			number /= i;
			sq = (long long) sqrt(number);
			i = 2;
		} else {
			//printf("mod!=0 i=%lld\n", i);
			i++;
		}
	}
	printf("mod=0 i=%lld\n", number);

	return 0;
}

