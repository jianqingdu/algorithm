//
//  binary_search_tree.c
//  algorithm
//
//  Created by jianqing.du on 15-12-11.
//  Copyright (c) 2015年. All rights reserved.
//

/*
 * implement binary search tree in <<Introduction to Algorithm>> 3rd Edition, chapter 12
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    int key;
    struct node* parent;
    struct node* left;
    struct node* right;
} node_t;

void inorder_tree_walk(node_t* root)
{
    if (root) {
        inorder_tree_walk(root->left);
        printf("%d ", root->key);
        inorder_tree_walk(root->right);
    }
}

node_t* tree_search(node_t* root, int key)
{
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key) {
        return tree_search(root->left, key);
    } else {
        return tree_search(root->right, key);
    }
}

node_t* iterative_tree_search(node_t* root, int key)
{
    node_t* n = root;
    while (n && (key != n->key)) {
        if (key < n->key) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    
    return n;
}

node_t* tree_minimum(node_t* root)
{
    node_t* n = root;
    while (n->left) {
        n = n->left;
    }
    
    return n;
}

node_t* tree_maximum(node_t* root)
{
    node_t* n = root;
    while (n->right) {
        n = n->right;
    }
    
    return n;
}

node_t* tree_succesor(node_t* n)
{
    if (n->right) {
        return tree_minimum(n->right);
    }
    
    node_t* p = n->parent;
    while (p && p->right == n) {
        n = p;
        p = p->parent;
    }
    
    return p;
}

node_t* tree_insert(node_t* root, int key)
{
    node_t* parent = NULL;
    node_t* n = root;
    
    while (n) {
        parent = n;
        
        if (key < n->key) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    
    node_t* new_n = malloc(sizeof(node_t));
    assert(new_n != NULL);
    new_n->key = key;
    new_n->parent = parent;
    new_n->left = new_n->right = NULL;
    
    // root is NULL
    if (parent == NULL) {
        root = new_n;
        return root;
    }
    
    if (key < parent->key) {
        parent->left = new_n;
    } else {
        parent->right = new_n;
    }
    
    return root;
}

// return root
node_t* transplant(node_t* root, node_t* u, node_t* v)
{
    if (u->parent == NULL) {
        return v;
    } else if (u->parent->left == u) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != NULL) {
        v->parent = u->parent;
    }
    
    return root;
}

node_t* tree_delete(node_t* root, int key)
{
    node_t* z = tree_search(root, key);
    if (!z) {
        return root;
    }
    
    if (z->left == NULL) {
        root = transplant(root, z, z->right);
    } else if (z->right == NULL) {
        root = transplant(root, z, z->left);
    } else {
        node_t* y = tree_minimum(z->right);
        if (y->parent != z) {
            root = transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        root = transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    
    free(z);
    return root;
}

int main(int argc, char* argv[])
{
    node_t* root = NULL;
    root = tree_insert(root, 12);
    root = tree_insert(root, 10);
    root = tree_insert(root, 16);
    root = tree_insert(root, 6);
    root = tree_insert(root, 18);
    root = tree_insert(root, 15);
    
    node_t* n = tree_search(root, 10);
    if (n) {
        printf("find 10\n");
        
        node_t* next = tree_succesor(n);
        if (next) {
            printf("next of 10 is %d\n", next->key);
        }
    }
    
    root = tree_delete(root, 16);
    
    printf("inorder_tree_walk\n");
    inorder_tree_walk(root);
    printf("\n");
    
    return 0;
}
