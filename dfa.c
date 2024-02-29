#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfa.h"

char * dfa_run(fa_t * dfa, const char * input)
{
    char * state = dfa->initial;
    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        set_t * transitions_for_state = set_find(dfa->transitions, state);
        if (!transitions_for_state) {
            fprintf(stderr, "No transitions defined for state '%s'!\n", state);
            return NULL;
        }
        set_t * next_state = set_find(transitions_for_state, single_char_str(c));
        if (next_state == NULL || next_state->len == 0) {
            fprintf(stderr, "No transition for char '%c' defined for state '%s'!\n", c, state);
            return NULL;
        }
        state = next_state->data[0].key;
    }
    return state;
}

bool dfa_accepts(fa_t * dfa, const char * input)
{
    return set_find(dfa->accepted, dfa_run(dfa, input));
}