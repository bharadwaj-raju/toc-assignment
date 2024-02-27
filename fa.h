#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FA_H
#define FA_H

#include "set.h"

typedef struct fa
{
    char initial[KEY_LEN];
    set_t * accepted;
    set_t * transitions;
    set_t * lambda_transitions;
} fa_t;

typedef struct fa_transition
{
    char letter;
    char next[KEY_LEN];
} fa_transition_t;

char * single_char_str(char c)
{
    char * str = (char *)malloc(sizeof(char) * 2);
    str[0] = c;
    str[1] = '\0';
    return str;
}

void fa_add_transition(fa_t * fa, const char * state1, char c, const char * state2)
{
    set_t * transitions = set_find(fa->transitions, state1);
    if (!transitions) {
        transitions = set_init();
        set_add(fa->transitions, state1, transitions);
    }
    char * c_str = single_char_str(c);
    set_t * char_transitions = set_find(transitions, c_str);
    if (!char_transitions) {
        char_transitions = set_init();
        set_add(transitions, c_str, char_transitions);
    }
    set_add(char_transitions, state2, &PRESENT);
}

void fa_add_lambda_transition(fa_t * fa, const char * state1, const char * state2)
{
    set_add(fa->lambda_transitions, state2, &PRESENT);
}

#endif