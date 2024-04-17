#ifndef PDA_H
#define PDA_H

#include <stdbool.h>
#include <stdio.h>

#include "../common/set.h"
#include "../common/stack.h"

#define EPSILON ' '

typedef struct pda
{
    char initial[KEY_LEN];
    // set of strings
    set_t * accepted;
    // the transition function depends on the state
    // so we store it as a set of string "<state> (<input char>/<stack char>)" -> set of string "state (<stack char>)"
    set_t * transitions;
} pda_t;

typedef struct pda_branch
{
    char state[KEY_LEN];
    stack_t * stack;
    size_t input_pos;
} pda_branch_t;

void pda_set_verbose(bool v);
pda_t * pda_init();
pda_t * pda_from_file(FILE * fp);
void pda_to_file(pda_t * pda, FILE * fp);
void pda_set_initial(pda_t * pda, const char * initial);
void pda_add_accepted(pda_t * pda, const char * accepted);
void pda_add_transition(pda_t * pda, const char * from_state, char inputc, char stackc, const char * to_state, char to_stackc);
bool pda_recognize(pda_t * pda, const char * input);

pda_branch_t * pda_branch_create(const char * state, size_t input_pos, stack_t * stack);

#endif