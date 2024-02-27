#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FA_H
#define FA_H

#include "tree.h"

typedef struct fa
{
    char initial[TREE_KEY_MAX_LEN];
    tree_node_t * accepted;
    tree_node_t * transitions;
    tree_node_t * lambda_transitions;
} fa_t;

char * single_char_str(char c)
{
    char * str = (char *)malloc(sizeof(char) * 2);
    str[0] = c;
    str[1] = '\0';
    return str;
}

void fa_add_transition(fa_t * fa, const char * state1, char c, const char * state2)
{
    tree_node_t * map_node = tree_find(fa->transitions, state1);
    if (!map_node) {
        map_node = tree_node_create(state1, NULL);
        tree_insert(&fa->transitions, tree_node_create(state1, NULL));
    }
    char * state2_copy = (char *)malloc(sizeof(char) * TREE_KEY_MAX_LEN);
    strncpy(state2_copy, state2, TREE_KEY_MAX_LEN);
    tree_insert((tree_node_t **)(&map_node->data), tree_node_create(single_char_str(c), state2_copy));
}

void fa_add_lambda_transition(fa_t * fa, const char * state1, const char * state2)
{
    char * state2_copy = (char *)malloc(sizeof(char) * TREE_KEY_MAX_LEN);
    strncpy(state2_copy, state2, TREE_KEY_MAX_LEN);
    tree_insert(&fa->lambda_transitions, tree_node_create(state1, state2_copy));
}

#endif