#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

typedef struct
{
  int num;
} item_t;



int
node_cmp (const void *a, const void *b)
{
  item_t *a_p = (item_t *) a;
  item_t *b_p = (item_t *) b;
  int ret;

  //printf ("node_cmp(%d, %d) ", a_p->num, b_p->num);

  if (a_p->num < b_p->num)
    ret = -1;
  else if (a_p->num > b_p->num)
    ret = 1;
  else
    ret = 0;

  //printf (" ret=%d\n", ret);

  return ret;
}


int
main (void)
{
  heap_t hp;
  item_t *item_p;
  int val;

  hp = heap_new ();
  
  item_p = malloc (sizeof (item_t));
  item_p->num = 9;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 0;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 0;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 9;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 100;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 7;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 77;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 2;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 4;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 56;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 10;  heap_insert (hp, item_p, item_p->num);
  item_p = malloc (sizeof (item_t));
  item_p->num = 100;  heap_insert (hp, item_p, item_p->num);

  while (1) 
    {
      if (heap_pop (hp, (void **) &item_p, &val) <= 0)
	break;
      printf ("num=%d, val=%d\n", item_p->num, val);
      free (item_p);
      //heap_pop (hp, (void **) &item_p, &val);
    }

  heap_destroy (hp);
  
  return 0;
}
