#include <stdio.h>
#include <ctype.h>

long pow(int bot, int exp)
{
  if (exp == 0)
    return 1;
  else 
    return bot * pow(bot, exp - 1);
}

int convert()
{
  char array[100];
  char ch;
  int i = 0, j;
  long n = 0;

  while((ch = getc(stdin)) != ' ' && ch != '\n') {
    if (!isdigit(ch))
      break;
    array[i++] = ch - '0';
  }
  for (j = 0; j < i; j++)
    printf("%c",array[j]);

  for (j = 0; j < i; j++) {
    n = n + (array[j] * pow(10, i - j - 1));
    printf("\n%d\t%d", array[j], n);
  }
  printf("\n%d", n);
}

main() 
{
  convert();
}





