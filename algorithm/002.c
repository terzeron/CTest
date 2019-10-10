#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char mine_map[100][100];
char cnt_map[100][100];

void increment(int i, int j)
{
    printf("increment(%d, %d)\n", i, j);
    if (cnt_map[i][j] >= '0' && cnt_map[i][j] <= '7') {
	++cnt_map[i][j];
    }
}


void fill_around_mine(int i, int j, int width, int height)
{
    int m;
    int n;

    printf("fill_around_mine(%d, %d)\n", i, j);
    /* (m, n) --> (i-1, j-1) (i-1, j) (i-1, j+1) 
                  (i,   j-1) (i,   j) (i,   j+1)
                  (i+1, j-1) (i+1, j) (i+1, j+1) */
    for (m = i - 1; m <= i + 1; ++m) {
	for (n = j - 1; n <= j + 1; ++n) {
	    if (m >= 0 && m < width && n >= 0 && n < width) {
		if (m == i && n == j)
		    cnt_map[i][j] = '*';
		else 
		    increment(m, n);
	    }
	}
    }
}

void print_cnt_map(int field_num, int width, int height)
{
    int i;
    int j;

    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; ++j) {
	    if (mine_map[i][j] == '*') {
		fill_around_mine(i, j, width, height);
	    }
	}
    }

    printf("Field #%d:\n", field_num);
    for (i = 0; i < height; ++i) {
	for (j = 0; j < width; ++j) {
	    printf("%c", cnt_map[i][j]);
	}
	printf("\n");
    }
}

int main(void)
{
    int width;
    int height;
    int i;
    char mine_str[100];
    int j;
    int field_num = 0;

    while (scanf("%d%d", &height, &width) != EOF) {
	if (width == 0 && height == 0) 
	    break;
	memset(mine_map, 0, sizeof (char) * 100 * 100);
	memset(cnt_map, '0', sizeof (char) * 100 * 100);
	for (i = 0; i < height; ++i) {
	    if (scanf("%s", mine_str) == EOF) {
		break;
	    }
	    if (strlen(mine_str) != width) {
		fprintf(stderr, "invalid width!\n");
		break;
	    }
	    for (j = 0; j < width; ++j) {
		if (mine_str[j] != '.' && mine_str[j] != '*') {
		    fprintf(stderr, "invalid character!\n");
		    break;
		}
	    }
	    strcpy(mine_map[i], mine_str);
	}
	print_cnt_map(++field_num, width, height);

	printf("\n");
    }
    
    return 0;
}
