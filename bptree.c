//
//  mybptree.c
//  bptree
//
//  Created by jianqing.du on 15-12-1.
//  Copyright (c) 2015年. All rights reserved.
//

/*
 B+ Tree Implementation as described in <<Database System Concept>> 6th Edition chapter 11.3
 order is the max pointer number in one node
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_ORDER   4

typedef struct record {
    int value;
} record_t;

typedef struct node {
    void**  pointers;
    int*    keys;
    int     num_keys;
    bool    is_leaf;
    struct node* parent;
    struct node* next;
} node_t;

// help function for print_tree, avoid recurive calls
void enqueue(node_t** queue, node_t* n)
{
    if (!*queue) {
        *queue = n;
        n->next = NULL;
    } else {
        node_t* c = *queue;
        while (c->next) {
            c = c->next;
        }
        
        c->next = n;
        n->next = NULL;
    }
}

node_t* dequeue(node_t** queue)
{
    node_t* n = NULL;
    if (*queue) {
        n = *queue;
        *queue = n->next;
    }
    
    return n;
}

node_t* make_node()
{
    node_t* n = malloc(sizeof(node_t));
    if (n == NULL) {
        perror("malloc failed\n");
        exit(1);
    }
    
    n->pointers = malloc(DEFAULT_ORDER * sizeof(void*));
    if (n->pointers == NULL) {
        perror("malloc failed\n");
        exit(1);
    }
    
    n->keys = malloc(DEFAULT_ORDER * sizeof(int));
    if (n->keys == NULL) {
        perror("malloc failed\n");
        exit(1);
    }
    
    n->num_keys = 0;
    n->is_leaf = false;
    n->parent = NULL;
    n->next = NULL;
    
    return n;
}

void free_node(node_t* n)
{
    free(n->keys);
    free(n->pointers);
    free(n);
}

node_t* make_leaf()
{
    node_t* n = make_node();
    n->is_leaf = true;
    return n;
}

record_t* make_record(int value)
{
    record_t* record = malloc(sizeof(record_t));
    if (!record) {
        perror("malloc failed\n");
        return NULL;
    }
    
    record->value = value;
    return record;
}

int level_to_root(node_t* root, node_t* n)
{
    int level = 0;
    while (n != root) {
        level++;
        n = n->parent;
    }
    
    return level;
}

void print_tree(node_t* root)
{
    if (!root) {
        printf("empty tree\n");
        return;
    }
    
    int cur_level = 0;
    node_t* queue = NULL;
    enqueue(&queue, root);
    
    while (queue != NULL) {
        node_t* n = dequeue(&queue);
        
        if (n->parent && (n->parent->pointers[0] == n)) {
            // 一点优化，只有node是parent的第一个子节点才判断是否开始了下一层
            int level = level_to_root(root, n);
            if (level > cur_level) {
                printf("\n");
                cur_level = level;
            }
        }
        
        //printf("*(0x%lx)* ", (long)n);
        for (int i = 0; i < n->num_keys; i++) {
            printf(" %d ", n->keys[i]);
        }
        //if (!n->is_leaf)
        //    printf("(0x%lx)", (long)n->pointers[n->num_keys]);
        printf("|");
        
        if (!n->is_leaf) {
            for (int i = 0; i <= n->num_keys; i++) {
                enqueue(&queue, n->pointers[i]);
            }
        }
    }
    
    printf("\n");
}

node_t* find_leaf(node_t* root, int key)
{
    if (!root) {
        return NULL;
    }
    
    node_t* c = root;
    while (!c->is_leaf) {
        int i = 0;
        for ( ; i < c->num_keys; ++i) {
            if (key < c->keys[i]) {
                break;
            }
        }
        
        c = c->pointers[i];
    }
    
    return c;
}

record_t* find(node_t* root, int key)
{
    node_t* l = find_leaf(root, key);
    if (!l) {
        return NULL;
    }
    
    int i = 0;
    for ( ; i < l->num_keys; i++) {
        if (key == l->keys[i]) {
            return l->pointers[i];
        }
    }
    
    return NULL;
}

int cut(int order)
{
    if (order % 2 == 0) {
        return order / 2;
    } else {
        return order / 2 + 1;
    }
}

node_t* start_new_root(int key, int value)
{
    node_t* root = make_leaf();
    record_t* record = make_record(value);
    
    root->keys[0] = key;
    root->pointers[0] = record;
    root->num_keys = 1;
    return root;
}

int calc_insert_index(node_t* n, int key)
{
    int insert_idx = n->num_keys;
    for (int i = 0; i < n->num_keys; i++) {
        if (key < n->keys[i]) {
            insert_idx = i;
            break;
        }
    }
    
    return insert_idx;
}

void insert_in_leaf(node_t* l, int key, int value)
{
    record_t* record = make_record(value);
    
    int insert_idx = calc_insert_index(l, key);
    
    for (int i = l->num_keys; i > insert_idx; i--) {
        l->keys[i + 1] = l->keys[i];
        l->pointers[i + 1] = l->pointers[i];
    }
    
    l->keys[insert_idx] = key;
    l->pointers[insert_idx] = record;
    l->num_keys++;
}

void insert_in_node(node_t* n, int key, void* pointer)
{
    int insert_idx = calc_insert_index(n, key);
    
    for (int i = n->num_keys; i > insert_idx; i--) {
        n->keys[i + 1] = n->keys[i];
    }
    n->keys[insert_idx] = key;
    
    // pointer index is 1 offset from key
    for (int i = n->num_keys + 1; i > insert_idx + 1; i--) {
        n->pointers[i + 1] = n->pointers[i];
    }
    n->pointers[insert_idx + 1] = pointer;
    
    n->num_keys++;
}

node_t* insert_in_parent(node_t* root, node_t* left, int key, node_t* right)
{
    if (left == root) {
        node_t* new_root = make_node();
        
        new_root->keys[0] = key;
        new_root->pointers[0] = left;
        new_root->pointers[1] = right;
        new_root->num_keys = 1;
        left->parent = new_root;
        right->parent = new_root;
        return new_root;
    }
    
    node_t* P = left->parent;
    if (P->num_keys < DEFAULT_ORDER - 1) {
        insert_in_node(P, key, right);
        return root;
    }
    
    // case: no space in internal node, split the internal node
    
    // copy P and (key, right) to tempory memory
    int* tmp_keys = malloc(DEFAULT_ORDER * sizeof(int));
    void** tmp_pointers = malloc((DEFAULT_ORDER + 1) * sizeof(void*));
    if (!tmp_keys || !tmp_pointers) {
        perror("malloc failed\n");
        exit(1);
    }
    
    int insert_idx = calc_insert_index(P, key);
    int i = 0;
    int j = 0;
    for (i = 0, j = 0; i < P->num_keys; i++, j++) {
        if (i == insert_idx) {
            j++;
            continue;
        }
        
        tmp_keys[j] = P->keys[i];
    }
    
    for (i = 0, j = 0; i < P->num_keys + 1; i++, j++) {
        if (i == (insert_idx + 1)) {
            j++;
            continue;
        }
        
        tmp_pointers[j] = P->pointers[i];
    }
    
    tmp_keys[insert_idx] = key;
    tmp_pointers[insert_idx + 1] = right;
    
    // erase pointer in P
    for (i = 0; i < P->num_keys + 1; i++) {
        P->pointers[i] = NULL;
    }
    
    // create a new node
    node_t* P_prime = make_node();
    P_prime->parent = P->parent;
    
    // copy from tmp_keys, tmp_pointers to P and P_prime
    int split_idx = cut(DEFAULT_ORDER);
    for (i = 0; i < split_idx - 1; i++) {
        P->keys[i] = tmp_keys[i];
        P->pointers[i] = tmp_pointers[i];
    }
    P->pointers[i] = tmp_pointers[i];
    P->num_keys = split_idx - 1;
    
    int k_prime = tmp_keys[split_idx - 1];
    
    for (i++, j = 0; i < DEFAULT_ORDER; i++, j++) {
        P_prime->keys[j] = tmp_keys[i];
        P_prime->pointers[j] = tmp_pointers[i];
    }
    P_prime->pointers[j] = tmp_pointers[i];
    P_prime->num_keys = DEFAULT_ORDER - split_idx;
    
    // set all child of P_prime's parent to P_prime
    for (i = 0; i < P_prime->num_keys + 1; i++) {
        node_t* child = P_prime->pointers[i];
        child->parent = P_prime;
    }
    
    free(tmp_keys);
    free(tmp_pointers);
    
    return insert_in_parent(root, P, k_prime, P_prime);
}

node_t* insert(node_t* root, int key, int value)
{
    // case: the first key in the root
    if (!root) {
        return start_new_root(key, value);
    }
    
    // no duplicate key allowed
    record_t* record = find(root, key);
    if (record) {
        return root;
    }
    
    node_t* L = find_leaf(root, key);
    if (!L) {
        return root;
    }
    
    // case: have space in leaf node
    if (L->num_keys < DEFAULT_ORDER - 1) {
        insert_in_leaf(L, key, value);
        return root;
    }
    
    // case: no space in leaf node, split the leaf
    node_t* L_prime = make_leaf();
    L_prime->parent = L->parent;
    
    // copy leaf and (key,value) to tempory memory of (key,pointer) pairs
    int* tmp_keys = malloc(DEFAULT_ORDER * sizeof(int));
    void** tmp_pointers = malloc(DEFAULT_ORDER * sizeof(void*));
    if (!tmp_keys || !tmp_pointers) {
        perror("malloc failed\n");
        exit(1);
    }
    
    record = make_record(value);
    int insert_index = L->num_keys;
    int i = 0;
    int j = 0;
    for (; i < L->num_keys; i++, j++) {
        if ((key < L->keys[i]) && (insert_index != L->num_keys)) {
            insert_index = j++;
        }
        
        tmp_keys[j] = L->keys[i];
        tmp_pointers[j] = L->pointers[i];
    }
    
    tmp_keys[insert_index] = key;
    tmp_pointers[insert_index] = record;
    
    // set last pointer
    L_prime->pointers[DEFAULT_ORDER - 1] = L->pointers[DEFAULT_ORDER - 1];
    L->pointers[DEFAULT_ORDER - 1] = L_prime;
    
    // erase pointer in L
    for (i = 0; i < L->num_keys; i++) {
        L->pointers[i] = NULL;
        L->num_keys = 0;
    }
    
    // copy from tmp_keys, tmp_pointers to L and L_prime
    int split_idx = cut(DEFAULT_ORDER);
    for (i = 0; i < split_idx; i++) {
        L->keys[i] = tmp_keys[i];
        L->pointers[i] = tmp_pointers[i];
    }
    L->num_keys = split_idx;
    
    for (i = split_idx, j = 0; i < DEFAULT_ORDER; i++, j++) {
        L_prime->keys[j] = tmp_keys[i];
        L_prime->pointers[j] = tmp_pointers[i];
    }
    L_prime->num_keys = DEFAULT_ORDER - split_idx;
    
    int k_prime = L_prime->keys[0];
    
    free(tmp_keys);
    free(tmp_pointers);
    
    return insert_in_parent(root, L, k_prime, L_prime);
}
/////////////

void delete_in_node(node_t* n, int key, void* pointer)
{
    int i = 0;
    
    // remove key and shift keys
    while (key != n->keys[i]) {
        ++i;
    }
    
    for (++i; i < n->num_keys; i++) {
        n->keys[i - 1] = n->keys[i];
    }
 
    // remove pointers and shift pointers
    int num_pointers = n->is_leaf ? n->num_keys : n->num_keys + 1;
    i = 0;
    while (pointer != n->pointers[i]) {
        ++i;
    }
    
    for (++i; i < num_pointers; i++) {
        n->pointers[i - 1] = n->pointers[i];
    }
    n->pointers[i - 1] = NULL;
    
    n->num_keys--;
}

node_t* adjust_root(node_t* root)
{
    if (root->num_keys > 0) {
        return root;
    }
    
    node_t* new_root = root->pointers[0];
    new_root->parent = NULL;
    
    free(root->keys);
    free(root->pointers);
    free(root);
    return new_root;
}

int get_neighbor_index(node_t* n)
{
    node_t* p = n->parent;
    
    int idx = 0;
    for (int i = 0; i < p->num_keys + 1; i++) {
        if (n == p->pointers[i]) {
            idx = i - 1;
            break;
        }
    }
    
    return idx;
}

node_t* delete_entry(node_t* root, node_t* N, int key, void* pointer);

node_t* coalesce_nodes(node_t* root, node_t* N, int k_prime, node_t* N_prime, int neighbor_idx)
{
    if (neighbor_idx == -1) {
        // swap the node so than N_prime is always the node before N
        node_t* tmp = N_prime;
        N_prime = N;
        N = tmp;
    }
    
    int i = 0;
    int j = 0;
    if (!N->is_leaf) {
        // append K_prime and all keys and pointer in N to N_prime
        N_prime->keys[N_prime->num_keys] = k_prime;
        N_prime->num_keys++;
        
        for (i = N_prime->num_keys, j = 0; j < N->num_keys; i++, j++) {
            N_prime->keys[i] = N->keys[j];
            N_prime->pointers[i] = N->pointers[j];
            N_prime->num_keys++;
        }
        
        N_prime->pointers[N_prime->num_keys] = N->pointers[N->num_keys];
    } else {
        // append all keys and point in N to N_prime
        for (i = N_prime->num_keys, j = 0; j < N->num_keys; i++, j++) {
            N_prime->keys[i] = N->keys[j];
            N_prime->pointers[i] = N->pointers[j];
            N_prime->num_keys++;
        }
        
        N_prime->pointers[DEFAULT_ORDER - 1] = N->pointers[DEFAULT_ORDER - 1];
    }
    
    root = delete_entry(root, N->parent, k_prime, N);
    free_node(N);
    return root;
}

// borrow an entry for N_prime
node_t* redistribute_nodes(node_t* root, node_t* N, int k_prime, node_t* N_prime, int neighbor_idx)
{
    int m = 0;
    if (neighbor_idx != -1) {
        // N_prime is a predecessor of N
        if (!N->is_leaf) {
            m = N_prime->num_keys;
            
            // shift all N (key,pointers) right 1 position
            N->pointers[N->num_keys + 1] = N->pointers[N->num_keys];
            for (int i = N->num_keys; i > 0; i--) {
                N->keys[i] = N->keys[i - 1];
                N->pointers[i] = N->pointers[i - 1];
            }
            
            N->keys[0] = N_prime->keys[m - 1];
            N->pointers[0] = N_prime->pointers[m];
            
            node_t* tmp = N->pointers[0];
            tmp->parent = N;
            
            N->parent->keys[neighbor_idx] = N->keys[0];
        } else {
            m = N_prime->num_keys - 1;
            
            for (int i = N->num_keys; i > 0; i--) {
                N->keys[i] = N->keys[i - 1];
                N->pointers[i] = N->pointers[i - 1];
            }
            
            N->keys[0] = N_prime->keys[m];
            N->pointers[0] = N_prime->pointers[m];
            
            N->parent->keys[neighbor_idx] = N->keys[0];
        }
    } else {
        // N_prime is a successor of N
        if (!N->is_leaf) {
            N->keys[N->num_keys] = k_prime;
            N->pointers[N->num_keys + 1] = N_prime->pointers[0];
            N->parent->keys[0] = N_prime->keys[0];
            
            node_t* tmp = N->pointers[N->num_keys + 1];
            tmp->parent = N;
        } else {
            N->keys[N->num_keys] = N_prime->keys[0];
            N->pointers[N->num_keys] = N_prime->pointers[0];
            N->parent->keys[0] = N_prime->keys[1];
        }
        
        int i = 0;
        for ( ;i < N_prime->num_keys - 1; i++) {
            N_prime->keys[i] = N_prime->keys[i + 1];
            N_prime->pointers[i] = N_prime->pointers[i + 1];
        }
        
        if (!N_prime->is_leaf) {
            N_prime->pointers[i] = N_prime->pointers[i + 1];
        }
    }
    
    N->num_keys++;
    N_prime->num_keys--;
    
    return root;
}

node_t* delete_entry(node_t* root, node_t* N, int key, void* pointer)
{
    delete_in_node(N, key, pointer);
    
    // case: only 1 pointer in root
    if (N == root) {
        return adjust_root(root);
    }
        
    int min_key = N->is_leaf ? cut(DEFAULT_ORDER - 1) : cut(DEFAULT_ORDER) - 1;
    
    // case: enough keys in the node
    if (N->num_keys >= min_key) {
        return root;
    }
    
    // case: not enough keys in the node
    int neighbor_idx = get_neighbor_index(N);
    node_t* N_prime = (neighbor_idx == -1) ? N->parent->pointers[1] : N->parent->pointers[neighbor_idx];
    int k_prime = (neighbor_idx == -1) ? N->parent->keys[0] : N->parent->keys[neighbor_idx];
    
    int capacity = N->is_leaf ? DEFAULT_ORDER : (DEFAULT_ORDER - 1);
    
    if (N->num_keys + N_prime->num_keys < capacity) {
        return coalesce_nodes(root, N, k_prime, N_prime, neighbor_idx);
    } else {
        return redistribute_nodes(root, N, k_prime, N_prime, neighbor_idx);
    }
}

node_t* delete(node_t* root, int key)
{
    node_t* leaf = find_leaf(root, key);
    record_t* record = find(root, key);
    
    if (leaf && record) {
        root = delete_entry(root, leaf, key, record);
        free(record);
    }
    
    return root;
}

// for test
int main(int argc, char* argv[])
{
    int max_num = 10;
    if (argc >= 2) {
        max_num = atoi(argv[1]);
    }
    
    node_t* root = NULL;
    for (int i = 1; i < max_num; i++) {
        root = insert(root, i, i);
    }
    print_tree(root);
    
    printf("---------\n\n");
    
    int min_num = 1;
    if (argc >= 3) {
        min_num = atoi(argv[2]);
    }
    for (int i = 1; i < min_num; i++) {
        root = delete(root, i);
    }
    
    print_tree(root);
    
    return 0;
}


