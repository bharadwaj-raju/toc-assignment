#ifndef FA_H
#define FA_H

#include "set.h"

extern int PRESENT;

typedef struct fa
{
    char initial[KEY_LEN];
    // set of strings
    set_t * accepted;
    // set of from_states -> (set of symbols -> (set of to_states))
    set_t * transitions;
    // set of from_states -> (set of to_states)
    set_t * lambda_transitions;
} fa_t;

fa_t * fa_init();
fa_t * fa_from_file(FILE * fp);
void fa_to_file(fa_t * fa, FILE * fp);
void fa_set_initial(fa_t * fa, const char * state);
void fa_add_accepted(fa_t * fa, const char * state);
void fa_add_transition(fa_t * fa, const char * from, char c, const char * to);
void fa_add_lambda_transition(fa_t * fa, const char * from, const char * to);

#endif