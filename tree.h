#include <stdlib.h>
#include <string.h>

#ifndef TREE_H
#define TREE_H

#define TREE_KEY_MAX_LEN 100

typedef struct tree_node
{
    char key[TREE_KEY_MAX_LEN];
    void * data;
    struct tree_node * left;
    struct tree_node * right;
    struct tree_node * parent;

} tree_node_t;

tree_node_t * tree_node_create(const char * key, void * data)
{
    tree_node_t * node = (tree_node_t *)(sizeof(tree_node_t));
    strncpy(node->key, key, TREE_KEY_MAX_LEN);
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void tree_insert(tree_node_t ** root, tree_node_t * node)
{
    if (!*root) {
        *root = node;
    } else {
        int cmp = strncmp((*root)->key, node->key, TREE_KEY_MAX_LEN);
        if (cmp == 0) {

        } else if (cmp > 0) {
            tree_insert(&(*root)->right, node);
            node->parent = *root;
        } else {
            tree_insert(&(*root)->left, node);
            node->parent = *root;
        }
    }
}

tree_node_t * tree_find(tree_node_t * root, const char * key)
{
    tree_node_t * curr = root;
    while (curr) {
        int cmp = strncmp(curr->key, key, TREE_KEY_MAX_LEN);
        if (cmp == 0) {
            break;
        } else if (cmp > 0) {
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }
    return curr;
}

void * tree_get(tree_node_t * root, const char * key)
{
    tree_node_t * node = tree_find(root, key);
    if (!node) {
        return NULL;
    }
    return node->data;
}

#endif