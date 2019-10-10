#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>


int hist[45][3] = { {39, 18}, {37, 17}, {40, 17}, {25, 16}, {26, 16}, {7, 15}, {2, 14}, {6, 14}, {16, 14}, {33, 14}, {1, 13}, {21, 13}, {36, 13}, {3, 12}, {8, 12}, {14, 12}, {15, 12}, {17, 12}, {30, 12}, {31, 12}, {35, 12}, {38, 12}, {42, 12}, {44, 12}, {4, 11}, {10, 11}, {19, 11}, {43, 11}, {9, 10}, {13, 10}, {18, 10}, {23, 10}, {27, 10}, {29, 9}, {5, 8}, {11, 8}, {20, 8}, {32, 8}, {41, 7}, {12, 6}, {22, 5}, {34, 5}, {45, 5}, {24, 4}, {28, 4} }; 

int comp(const void *a, const void *b)
{
    int x = *(int *) a;
    int y = *(int *) b;
    
    return (x > y ? 1 : (x == y ? 0 : -1));
}
    

int main()
{
	int sum = 0;
	int i, j;
	int selected_sum = 0;
	int rnd;
    int *candidate = NULL;
    int selected[6];
	
	srand(time(NULL));

    printf("making histogram...\n");
	for (i = 0; i < 45; i++) {
		sum += hist[i][1];
        hist[i][2] = sum;
	}

    printf("distributing candidates...\n");
    candidate = (int *) malloc(sizeof (int) * sum);
    for (i = 0, j = 0; i < sum && j < 45; i++) {
        if (i >= hist[j][2]) {
            j++;
        }
        candidate[i] = hist[j][0];
    }

    printf("selecting 6 numbers...\n");
	for (i = 0; i < 1000000000; i++) {
		selected_sum = 0;
		for (j = 0; j < 6; j++) {
			rnd = rand() % sum;
            //printf("rnd=%d ", rnd);
            //assert(rnd >= 0 && rnd < sum);
            selected[j] = candidate[rnd];
			selected_sum += selected[j];
            //printf("%d ", selected[j]);
		}
        //printf(" sum=%d\n", selected_sum);
        qsort(selected, 6, sizeof (int), comp);
        if (selected_sum == 138) {
            for (j = 0; j < 6; j++) {
                if (j > 0 && selected[j] == selected[j-1]) {
                    break;
                }
                printf("%d ", selected[j]);
            }
            if (j == 6) {
                printf("sum=%d\n", selected_sum);
            } else {
                printf("\n");
            }
        }
	}

	return 0;
}
