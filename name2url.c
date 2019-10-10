#include <stdio.h>

int main(void)
{
    int ch;

    while ((ch = getchar()) != EOF && ch != '\n') {
        if (ch & 0x80) {
            printf("%%%02X", ch);
        } else {
            printf("%c", ch);
        }
    }

    printf("\n");

    return 0;
}
