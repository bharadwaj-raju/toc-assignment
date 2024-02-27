#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DFA_H
#define DFA_H

#include "fa.h"
#include "tree.h"

char * dfa_run(fa_t * dfa, const char * input)
{
    char * state = dfa->initial;
    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        tree_node_t * map = (tree_node_t *)tree_get(dfa->transitions, state);
        if (!map) {
            fprintf(stderr, "No transitions defined for state '%s'!\n", state);
            return NULL;
        }
        char * next_state = (char *)tree_get(map, single_char_str(c));
        if (next_state == NULL) {
            fprintf(stderr, "No transition for char '%c' defined for state '%s'!\n", c, state);
            return NULL;
        }
        state = next_state;
    }
    return state;
}

#endif