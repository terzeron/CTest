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

struct fraction {
  int numerator;
  int denominator;
  int gcd;
}
  
main()
{
  struct fraction frac;
  
  printf("Input two numbers\n");
  while (scanf("%d %d", &frac.numerator, &frac.denominator) != EOF) {
    if (frac.numerator * frac.denominator == 0) {
      printf("There is no GCD between 0 and some number.\n");
      exit(-1);
    } 
    frac.gcd = gcd(frac.numerator, frac.denominator);;
    printf("%d / %d --> %d / %d\n", frac.numerator, frac.denominator, frac.numerator / frac.gcd, frac.denominator / frac.gcd);
  }
}
