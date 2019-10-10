#include <stdio.h>
#include <math.h>

int gcd(int u, int v)
{
  int t;

  while (u * v > 0) {
    if (abs(u) < abs(v)) {
      t = u;
      u = v;
      v = t;
    }
    u = u - v;
  }
  while (u * v < 0) {
    if (abs(u) < abs(v)) {
      t = u;
      u = v;
      v = t;
    }
    u = u + v;
  }
  return v;
}

main () 
{
  int x, y;

  printf("Input two numbers\n");
  while (scanf("%d %d", &x, &y) != EOF) {
    if (x * y == 0) {
      printf("There is no GCD of between 0 and some number.");
      exit(-1);
    }
    printf("%d %d %d\n", x, y, gcd(x, y));
  }
}

  






