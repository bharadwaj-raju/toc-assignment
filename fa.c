#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fa.h"

char * single_char_str(char c)
{
    char * str = (char *)malloc(sizeof(char) * 2);
    str[0] = c;
    str[1] = '\0';
    return str;
}

fa_t * fa_init()
{
    fa_t * fa = malloc(sizeof(fa_t));
    fa->accepted = set_init();
    fa->transitions = set_init();
    fa->lambda_transitions = set_init();
    fa->initial[0] = '\0';
    return fa;
}

void fa_set_initial(fa_t * fa, const char * state)
{
    strncpy(fa->initial, state, KEY_LEN);
}

void fa_add_accepted(fa_t * fa, const char * state)
{
    set_add(fa->accepted, state, &PRESENT);
}

void fa_add_transition(fa_t * fa, const char * from, char c, const char * to)
{
    // printf("adding trans %s (%c) -> %s\n", from, c, to);
    set_t * transitions = set_find(fa->transitions, from);
    if (!transitions) {
        transitions = set_init();
        set_add(fa->transitions, from, transitions);
    }
    char * c_str = single_char_str(c);
    set_t * char_transitions = set_find(transitions, c_str);
    if (!char_transitions) {
        char_transitions = set_init();
        set_add(transitions, c_str, char_transitions);
    }
    set_add(char_transitions, to, &PRESENT);
}

void fa_add_lambda_transition(fa_t * fa, const char * from, const char * to)
{
    set_t * lambda_transitions = set_find(fa->lambda_transitions, from);
    if (!lambda_transitions) {
        lambda_transitions = set_init();
        set_add(fa->lambda_transitions, from, lambda_transitions);
    }
    set_add(lambda_transitions, to, &PRESENT);
}