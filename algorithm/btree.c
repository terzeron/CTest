/*
 * B-Tree
 *
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct key key_t;
typedef struct node node_t;
struct key {
    int value;
    node_t *node;
};
struct node {
    unsigned int isleaf;
    unsigned int keysize; // child array size
    key_t **keyes; 
    node_t **children;
}; 

#define KEYSIZE(node) ((node)->keysize)
#define ISLEAF(node) ((node)->isleaf)
#define VALUE(key) ((key)->value)
#define KEYVALUE(node, i) ((node)->keyes[i]->value)
#define KEY(node, i) ((node)->keyes[i])
#define CHILD(node, i) ((node)->children[i])
#define NODE(key) ((key)->node)

node_t *root_node = NULL;
unsigned int min_degree = 2;

key_t *search(node_t *node, key_t *key)
{
    int i;
    
    printf("# search()\n");
    
    // linear search
    for (i = 0; i < KEYSIZE(node) && VALUE(key) > KEYVALUE(node, i); i++) 
        ;
    
    // 해당 키를 노드 안에서 찾았다면 해당 인덱스를 반환
    if (i < KEYSIZE(node) && VALUE(key) == KEYVALUE(node, i)) {
        NODE(key) = node;
        return KEY(node, i);
    }
        
    if (ISLEAF(node)) {
        return NULL;
    } else {
        // disk_read(c_i[node])
        return search(CHILD(node, i), key);
    }
}


node_t *make_newnode(void)
{
    int i;
    node_t *temp;

    printf("# make_newnode()\n");

    temp = (node_t *) malloc(sizeof (node_t));
    temp->children = (node_t **) malloc(sizeof (node_t *) * 
                                        (2 * min_degree));
    temp->keyes = (key_t **) malloc(sizeof (key_t *) * 
                                    (2 * min_degree - 1));
    KEYSIZE(temp) = 0;

    for (i = 0; i < 2 * min_degree; i++) {
        temp->children[i] = NULL;
    }
    
    for (i = 0; i < 2 * min_degree - 1; i++) {
        temp->keyes[i] = NULL;
    }

    return temp;
}


void create(void)
{
    printf("# create()\n");

    root_node = make_newnode();
    ISLEAF(root_node) = 1;

    return;
}


void split_child(node_t *parent, int i, node_t *child)
{
    int j;
    node_t *new_child = NULL;

    printf("# split_child()\n");

    printf("make new sibling\n");
    new_child = make_newnode();
    ISLEAF(new_child) = ISLEAF(child);
    KEYSIZE(new_child) = min_degree - 1;

    // key 분할
    printf("split keyes\n");
    for (j = 0; j < min_degree - 1; j++) {
        KEY(new_child, j) = KEY(child, j + min_degree);
    }

    // child 분할
    printf("split children\n");
    if (!ISLEAF(child)) {
        for (j = 0; j < min_degree; j++) {
            CHILD(new_child, j) = CHILD(child, j + min_degree);
        }
    }

    KEYSIZE(child) = min_degree - 1;
    
    // parent의 child를 오른쪽으로 1개씩 shift
    printf("shift children right\n");
    for (j = KEYSIZE(parent) + 1; j > i + 1; j--) {
        CHILD(parent, j + 1) = CHILD(parent, j);
    }

    // new_child를 위한 링크 설정
    CHILD(parent, i + 1) = new_child;
    
    // parent의 key를 오른쪽으로 1개씩 shift
    printf("shift keyes right\n");
    for (j = KEYSIZE(parent) + 1; j > i; j--) {
        KEY(parent, j + 1) = KEY(parent, j);
    }

    // parent로 올라간 child의 key 등록
    KEY(parent, i) = KEY(child, min_degree);

    KEYSIZE(parent)++;

    return;
}


void insert_notfull(node_t *node, key_t *key)
{
    int i = KEYSIZE(node) - 1;
    
    printf("# insert_notfull() keysize(node)=%d key=%c\n", i, VALUE(key));

    if (ISLEAF(node)) {
        printf("is leaf\n");
        // 해당 키보다 큰 값을 가지는 노드 안의 키들을 오른쪽으로 shift
        for ( ; i >= 0 && VALUE(key) < KEYVALUE(node, i); i--) {
            printf("in for i=%d\n", i);
            KEY(node, i + 1) = KEY(node, i);
        }
        printf("i=%d\n", i);

        // 리프 노드에 새로운 키 추가
        KEY(node, i + 1) = key;
        KEYSIZE(node)++;
    } else {
        printf("is not leaf\n");
        printf("i=%d\n", i);
        for ( ; i >= 0 && VALUE(key) < KEYVALUE(node, i); ) {
            i--;
            printf("in for i=%d\n", i); 
        }
        i++;
        printf("i=%d\n", i);
        
        if (KEYSIZE(CHILD(node, i)) == 2 * min_degree - 1) {
            split_child(node, i, CHILD(node, i));
            if (VALUE(key) > KEYVALUE(node, i)) {
                i++;
            }
        }
        insert_notfull(CHILD(node, i), key);
    }

    return;
}


void insert(node_t *node, key_t *key)
{
    node_t *old_root = root_node;
    node_t *new_root = NULL;

    printf("# insert() key=%c\n", VALUE(key));

    if (KEYSIZE(root_node) == 2 * min_degree- 1) {
        // 루트가 가득 찬 경우
        printf("make new root node\n");
        new_root = make_newnode();
        root_node = new_root;
        ISLEAF(new_root) = 0;
        CHILD(new_root, 0) = old_root;
  
        split_child(new_root, 0, old_root);
        insert_notfull(new_root, key);
    } else {
        // 루트가 가득 차지 않은 경우
        insert_notfull(old_root, key);
    }
}


int main(void)
{
    int i;
    int size = 10;
    key_t **keylist;
    node_t **nodelist;

    create();

    nodelist = (node_t **) malloc(sizeof (node_t *) * size);
    keylist = (key_t **) malloc(sizeof (key_t *) * size);
    for (i = 0; i < size; i++) {
        nodelist[i] = make_newnode();
        keylist[i] = (key_t *) malloc(sizeof (key_t));
        keylist[i]->value = 'A' + i;
        keylist[i]->node = nodelist[i];
        nodelist[i]->keyes[0] = keylist[i];
        nodelist[i]->keysize = 1;
    }

    insert(nodelist[9], KEY(nodelist[9], 0));
    insert(nodelist[8], KEY(nodelist[8], 0));
    insert(nodelist[2], KEY(nodelist[2], 0));
    insert(nodelist[1], KEY(nodelist[1], 0));
    insert(nodelist[3], KEY(nodelist[3], 0));
    insert(nodelist[0], KEY(nodelist[0], 0));
    insert(nodelist[6], KEY(nodelist[6], 0));
    insert(nodelist[4], KEY(nodelist[4], 0));
    insert(nodelist[7], KEY(nodelist[7], 0));
    insert(nodelist[5], KEY(nodelist[5], 0));

    return 0;
}



