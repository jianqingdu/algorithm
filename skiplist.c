//
//  skiplist.c
//  skiplist
//
//  Created by jianqing.du on 15-11-26.
//  Copyright (c) 2015年. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *  implement skip list in http://epaperpress.com/sortsearch/download/skiplist.pdf
 */

#define MAX_LEVEL 16

typedef struct node {
	int key;
	int value;
	struct node* next[];
} node_t;

typedef struct {
	int     level;
	node_t* header;
} skiplist_t;

// api 
skiplist_t* create_skiplist();
int sl_insert(skiplist_t* sl, int key, int value);
int sl_delete(skiplist_t* sl, int key);
int* sl_search(skiplist_t* sl, int key);

////////
static int rand_level()
{
    int level = 1;
    
    while ((rand() % 2 == 0) && (level < MAX_LEVEL)) {
        level++;
    }
    
    return level;
}

static node_t* new_node(int level, int key, int value)
{
    node_t* n = malloc(sizeof(node_t) + level * sizeof(node_t));
    if (!n) {
        return NULL;
    }
    
    n->key = key;
    n->value = value;
    return n;
}

skiplist_t* create_skiplist()
{
    srand((unsigned)time(NULL));
    
    skiplist_t* sl = malloc(sizeof(skiplist_t));
    if (!sl) {
        return NULL;
    }
    
    sl->level = 1;
    sl->header = new_node(MAX_LEVEL - 1, 0, 0);
    if (!sl->header) {
        return NULL;
    }
    
    for (int i = 0; i < MAX_LEVEL; ++i) {
        sl->header->next[i] = NULL;
    }
    
    return sl;
    
}

int sl_insert(skiplist_t* sl, int key, int value)
{
    node_t* update[MAX_LEVEL];
    node_t* current = sl->header;
    node_t* forward = NULL;
    
    int i;
    for (i = sl->level - 1; i >= 0; i--) {
        while ((forward = current->next[i]) && (forward->key < key)) {
            current = forward;
        }
        
        update[i] = current;
    }
    
    if (forward && forward->key == key) {
        forward->value = value;
        return 0;
    }
    
    int level = rand_level();
    if (level > sl->level) {
        for (i = sl->level; i < level; ++i) {
            update[i] = sl->header;
        }
        
        sl->level = level;
    }
    
    node_t* n = new_node(level, key, value);
    for (i = 0; i < level; i++) {
        n->next[i] = update[i]->next[i];
        
        update[i]->next[i] = n;
    }
    
    
    return 0;
}

int sl_delete(skiplist_t* sl, int key)
{
    node_t* update[MAX_LEVEL];
    node_t* current = sl->header;
    node_t* forward = NULL;
    
    int i;
    for (i = sl->level - 1; i >= 0; i--) {
        while ((forward = current->next[i]) && (forward->key < key)) {
            current = forward;
        }
        
        update[i] = current;
    }
    
    if (forward && forward->key == key) {
        for (i = 0; i < sl->level; ++i) {
            if (update[i]->next[i] != forward)
                break;
            
            update[i]->next[i] = forward->next[i];
        }
        
        free(forward);
        
        while ((sl->level >= 1) && (sl->header->next[sl->level - 1] == NULL)) {
            sl->level--;
        }
    }
    
    return 0;
}

int* sl_search(skiplist_t* sl, int key)
{
    node_t* current = sl->header;
    node_t* forward = NULL;
    
    int i;
    for (i = sl->level - 1; i >= 0; i--) {
        while ((forward = current->next[i]) && (forward->key < key)) {
            current = forward;
        }
    }
    
    if (forward && forward->key == key) {
        return &(forward->value);
    } else {
        return NULL;
    }
}

// for test
#define MAX_KEY 100
int main(int argc, const char * argv[])
{

    skiplist_t* sl = create_skiplist();

    for (int i = 0; i < MAX_KEY; i++) {
        sl_insert(sl, i, i);
    }

    for (int i = 0; i < MAX_KEY; i += 3) {
        sl_delete(sl, i);
    }

    for (int i = 0; i < MAX_KEY; i++) {
        int* p = sl_search(sl, i);
        if (p) {
            printf("get value(%d)=%d\n", i, *p);
        } else {
            printf("get value(%d)=NULL\n", i);
        }
    }

    return 0;
}
