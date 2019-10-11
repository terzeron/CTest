#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "heap.h"
//#include "ckd_alloc.h"


typedef struct heap_s
{
  void *data;			/* Application data at this node */
  int val;			/* Associated with above application data; according to which
				   heap is sorted (in ascending order) */
  int nl, nr;			/* #left/right descendants of this node (for balancing heap) */
  struct heap_s *l;		/* Root of left descendant heap */
  struct heap_s *r;		/* Root of right descendant heap */
} heapnode_t;


#if 0
static void
heap_dump (heapnode_t * top, int level)
{
  int i;

  if (!top)
    return;

  for (i = 0; i < level; i++)
    printf ("  ");
  /* print top info */
  heap_dump (top->l, level + 1);
  heap_dump (top->r, level + 1);
}
#endif


heap_t
heap_new (void)
{
  heapnode_t **h;

  h = (heapnode_t **) calloc (1, sizeof (heapnode_t *));
  *h = NULL;

  return ((heap_t) h);
}


static heapnode_t *
subheap_insert (heapnode_t * root, void *data, int val)
{
  heapnode_t *h;
  void *tmpdata;
  int tmpval;

  if (!root)
    {
      h = (heapnode_t *) malloc (sizeof (heapnode_t));
      h->data = data;
      h->val = val;
      h->l = h->r = NULL;
      h->nl = h->nr = 0;
      return h;
    }

  /* Root already exists; if new value is less, replace root node */
  if (root->val > val)
    {
      tmpdata = root->data;
      tmpval = root->val;
      root->data = data;
      root->val = val;
      data = tmpdata;
      val = tmpval;
    }

  /* Insert new or old (replaced) node in right or left subtree; keep them balanced */
  if (root->nl > root->nr)
    {
      root->r = subheap_insert (root->r, data, val);
      root->nr++;
    }
  else
    {
      root->l = subheap_insert (root->l, data, val);
      root->nl++;
    }

  return root;
}


int
heap_insert (heap_t heap, void *data, int val)
{
  heapnode_t **hp;

  hp = (heapnode_t **) heap;

  *hp = subheap_insert (*hp, data, val);

  return 0;
}


static heapnode_t *
subheap_pop (heapnode_t * root)
{
  heapnode_t *l, *r;

  /* Propagate best value from below into root, if any */
  l = root->l;
  r = root->r;

  if (!l)
    {
      if (!r)
	{
	  free ((char *) root);
	  return NULL;
	}
      else
	{
	  root->data = r->data;
	  root->val = r->val;
	  root->r = subheap_pop (r);
	  root->nr--;
	}
    }
  else
    {
      if ((!r) || (l->val < r->val))
	{
	  root->data = l->data;
	  root->val = l->val;
	  root->l = subheap_pop (l);
	  root->nl--;
	}
      else
	{
	  root->data = r->data;
	  root->val = r->val;
	  root->r = subheap_pop (r);
	  root->nr--;
	}
    }

  return root;
}


int
heap_pop (heap_t heap, void **data, int *val)
{
  heapnode_t **hp, *h;

  hp = (heapnode_t **) heap;
  h = *hp;

  if (!h)
    return 0;

  *data = h->data;
  *val = h->val;

  *hp = subheap_pop (h);

  return 1;
}


int
heap_top (heap_t heap, void **data, int *val)
{
  heapnode_t **hp, *h;

  hp = (heapnode_t **) heap;
  h = *hp;

  if (!h)
    return 0;

  *data = h->data;
  *val = h->val;

  return 1;
}


int
heap_destroy (heap_t heap)
{
  void *data;
  int val;

  /* Empty the heap and free it */
  while (heap_pop (heap, &data, &val) > 0);
  free ((char *) heap);

  return 0;
}
