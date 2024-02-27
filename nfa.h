#include <stdlib.h>
#include <string.h>

#ifndef NFA_H
#define NFA_H

#include "fa.h"
#include "tree.h"

const char * EMPTY = "";
int PRESENT = 100;

char * nfa_run(fa_t * fa)
{
    tree_node_t * reachable_states = NULL;
    tree_insert(&reachable_states, tree_node_create(fa->initial, &PRESENT));
}

void lambda_closure(fa_t * fa, const char * state)
{
}

void extend_by_lambda_closure(fa_t * fa, tree_node_t * states)
{
}

#endif