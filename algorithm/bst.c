/*
 * Binary Search Tree
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef warnx
#define warnx(msg) fprintf(stderr, msg ## "\n");
#endif

#ifndef errx
#define errx(fd, msg) fprintf(fd, msg ## "\n");
#endif

#define KEY(node) (((node) == NULL) ? -1 : (node)->key)
#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)
#define PARENT(node) ((node)->parent)

#if 1
#define search(node) search_iterative(node)
#else
#define search(node) search_recursive(root_node, node)
#endif

struct node {
    int key;
    void *data;
    struct node *left;
    struct node *right;
    struct node *parent;
};
typedef struct node node_t;
typedef struct node tree_t;

node_t *root_node = NULL;


int initialize(void)
{
    root_node = (node_t *) malloc(sizeof(node_t));
    if (root_node == NULL) {
	return -1;
    }

    PARENT(root_node) = NULL;
    LEFT(root_node) = NULL;
    RIGHT(root_node) = NULL;

    return 0;
}


int finalize(void)
{
    free(root_node);

    return 0;
}


int insert(node_t * node)
{
    node_t *temp_node = root_node;
    node_t *parent_node = NULL;

    printf("# insert %d\n", KEY(node));

    while (temp_node != NULL) {
	parent_node = temp_node;
	if (KEY(node) < KEY(temp_node)) {
	    temp_node = LEFT(temp_node);
	} else {
	    temp_node = RIGHT(temp_node);
	}
    }

    if (parent_node == NULL) {
	root_node = node;
    } else {
	if (KEY(node) < KEY(parent_node)) {
	    LEFT(parent_node) = node;
	} else {
	    RIGHT(parent_node) = node;
	}
	PARENT(node) = parent_node;
    }

    return 0;
}


node_t *search_iterative(node_t * node)
{
    node_t *temp_node = root_node;

    printf("# search %d: \n", KEY(node));

    while (temp_node != NULL && KEY(node) != KEY(temp_node)) {
	printf("%d-->", KEY(temp_node));

	if (KEY(node) < KEY(temp_node)) {
	    temp_node = LEFT(temp_node);
	} else {
	    temp_node = RIGHT(temp_node);
	}
    }

    printf("%d\n", KEY(temp_node));

    return temp_node;
}


node_t *search_recursive(node_t * start_node, node_t * node)
{
    printf("# search %d %d\n", KEY(start_node), KEY(node));

    if (start_node == NULL || KEY(node) == KEY(start_node)) {
	return start_node;
    }

    if (KEY(node) < KEY(start_node)) {
	return search_recursive(LEFT(start_node), node);
    } else {
	return search_recursive(RIGHT(start_node), node);
    }
}


node_t *minimum(node_t * node)
{
    node_t *temp_node = node;

    while (LEFT(temp_node) != NULL) {
	temp_node = LEFT(temp_node);
    }
    return temp_node;
}


node_t *maximum(node_t * node)
{
    node_t *temp_node = node;

    while (RIGHT(temp_node) != NULL) {
	temp_node = RIGHT(temp_node);
    }
    return temp_node;
}


node_t *successor(node_t * node)
{
    node_t *temp_node = node;
    node_t *temp_node2 = NULL;

    printf("# successor %d\n", KEY(node));

    // right subtree의 최소값을 찾음
    if (RIGHT(temp_node) != NULL) {
	temp_node = RIGHT(temp_node);

	while (LEFT(temp_node) != NULL) {
	    temp_node = LEFT(temp_node);
	}
	return temp_node;
    }

    temp_node2 = PARENT(temp_node);

    while (temp_node2 != NULL && temp_node == RIGHT(temp_node2)) {
	temp_node = temp_node2;
	temp_node2 = PARENT(temp_node2);
    }
    return temp_node2;
}


int delete(node_t * node)
{
    node_t *temp_node = NULL;
    node_t *temp_node2 = NULL;

    if (LEFT(node) == NULL || RIGHT(node) == NULL) {
	temp_node2 = node;
    } else {
	temp_node2 = successor(node);
    }

    if (LEFT(temp_node2) != NULL) {
	temp_node = LEFT(temp_node2);
    } else {
	temp_node = RIGHT(temp_node2);
    }

    if (temp_node != NULL) {
	PARENT(temp_node) = PARENT(temp_node2);
    }

    if (PARENT(temp_node2) == NULL) {
	root_node = temp_node;
    } else {
	if (temp_node2 == LEFT(PARENT(temp_node2))) {
	    LEFT(PARENT(temp_node2)) = temp_node;
	} else {
	    RIGHT(PARENT(temp_node2)) = temp_node;
	}
    }

    if (temp_node2 != node) {
	node->key = temp_node2->key;
	node->data = temp_node2->data;

	free(temp_node2);
    }

    return 0;
}


int main(void)
{
    unsigned int i;
    unsigned int size = 10;
    node_t *node = NULL;

    node = (node_t *) malloc(sizeof(node_t) * size);

    for (i = 0; i < size; i++) {
	node[i].key = i;
    }

    insert(&node[5]);
    insert(&node[2]);
    insert(&node[3]);
    insert(&node[4]);
    insert(&node[0]);
    insert(&node[1]);
    insert(&node[7]);
    insert(&node[6]);
    insert(&node[9]);
    insert(&node[8]);

    delete(&node[7]);

    for (i = 0; i < size; i++) {
	search(&node[i]);
	//printf("%d\n", KEY(successor(&node[i])));
    }

    return 0;
}
