#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
  int ch, i = 0;
  char name[100];
  int flag = 1;
  
  while ((ch = getchar()) != EOF) {
    if (ch == '\n') {
      name[i] = '\0';
      printf("%s\n", name);
      i = 0;
      continue;
    }
    
    if (i == 0 && isspace(ch)) {
      flag = 1;
      continue;
    }

    if (flag == 1 && !isspace(ch)) {
      if (i != 0)
	name[i++] = '_';
      ch = toupper(ch);
      flag = 0;
    }
    
    if (isspace(ch) || ch == '_' || ch == '-')
      flag = 1;
    
    if (flag == 0) {
      name[i++] = (char)ch;
    }
  }
  return 0;
}
