#include <stdio.h>
#include <string.h>
/* #include <iostream.h> */

int main(void)
{
    int i, k, count = 0;
    char str1[100], str2[100];
  
    printf("\nFirst : ");
    scanf("%s", str1);
    printf("\nSecond : ");
    scanf("%s", str2);
  
    printf("\nThe length of First: %lu\tSecond: %lu", strlen(str1), strlen(str2));
   
    for (i = 0; i < strlen(str1); i++) {
        for (k = 0; k < strlen(str2); k++)
            if (str1[i + k] == str2[k]) 
                count++;
        if (count == strlen(str2))
            printf("\nlocation : %d", i + 1);
        count = 0;
    }
    printf("\n");
    return 0;
}
 
