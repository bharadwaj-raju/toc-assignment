#ifndef SET_H
#define SET_H

#include "set.h"

typedef struct fa
{
    char initial[KEY_LEN];
    set_t * accepted;
    set_t * transitions;
    set_t * lambda_transitions;
} fa_t;

char * single_char_str(char c);
fa_t * fa_init();
void fa_set_initial(fa_t * fa, const char * state);
void fa_add_accepted(fa_t * fa, const char * state);
void fa_add_transition(fa_t * fa, const char * from, char c, const char * to);
void fa_add_lambda_transition(fa_t * fa, const char * from, const char * to);

#endif