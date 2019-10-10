void quicksort(int a[], int N)
{
  int i, l, r;

  l = 1;
  r = N;
  stackinit();

  for ( ; ; ) {
    while (r > l) {
      i = partition(a, l, r);
      if (i - 1 > r - i) {
        push(l);
        push(i - 1);
        l = i + 1;
      } else {
        push(i + 1);
        push(r);
        r = i - 1;
      }
    if (stackempty())
      break;
    r = pop();
    i = pop();
  }
}
