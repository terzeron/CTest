#define q 33554393
#define d 32

int rksearch(char *p, char *a)
{
  int i, dM = 1, h1 = 0, h2 = 0;
  int M, N;

  if (a == NULL || p == NULL) 
    return -1;
  else
    M = strlen(p), N = strlen(a);
 
  for (i = 1; i < M; i++) 
    dM = (d * dM) % q;
  for (i = 0; i < M; i++) {
    h1 = (h1 * d + index(p[i])) % q;
    h2 = (h2 * d + index(a[i])) % q;
  }
  for (i = 0; h1 != h2; i++) {
    h2 = (h2 + d * q - index(a[i]) * dM) % p;
    h2 = (h2 * d + index(a[i + M])) % p;
    if (i > N - M) 
      return N;
  }
  return i;
}
