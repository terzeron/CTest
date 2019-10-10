#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  int i, start, end, res;
  int key;
 
  scanf("%d", &key);
  start = 0;
  end = sizeof(arr) / sizeof(int) - 1;
  for (i = (start + end) / 2; start <= end; i = (start + end) / 2) {
    printf("start=%d end=%d arr[%d]=%d\n", start, end, i, arr[i]);
    res = (arr[i] - key);
    if (res == 0) {
      printf("Found!\n");
      break;
    } else if (res > 0) {
      end = i - 1;
    } else {
      start = i + 1;
    }
  }
  printf("start=%d end=%d arr[%d]=%d\n", start, end, i, arr[i]);
  return 0;
}
