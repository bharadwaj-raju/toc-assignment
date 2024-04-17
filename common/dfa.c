#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfa.h"

static bool verbose = false;

void dfa_set_verbose(bool v)
{
    verbose = v;
}

#define log(msg, ...) \
    if (verbose)      \
    fprintf(stdout, msg, ##__VA_ARGS__)

char * dfa_step(fa_t * dfa, char * state, char c)
{
    set_t * transitions_for_state = set_find(dfa->transitions, state);
    if (!transitions_for_state) {
        fprintf(stderr, "error: no transitions defined for state '%s'!\n", state);
        return NULL;
    }
    char * c_str = single_char_str(c);
    set_t * next_state = set_find(transitions_for_state, c_str);
    free(c_str);
    if (next_state == NULL || next_state->len == 0) {
        fprintf(stderr, "error: no transition for char '%c' defined for state '%s'!\n", c, state);
        return NULL;
    }
    return next_state->data[0].key;
}

char * dfa_run(fa_t * dfa, const char * input)
{
    char * state = dfa->initial;
    for (size_t i = 0; i < strlen(input); i++) {
        state = dfa_step(dfa, state, input[i]);
        if (!state) {
            return NULL;
        }
    }
    return state;
}

bool dfa_accepts(fa_t * dfa, const char * input)
{
    return set_find(dfa->accepted, dfa_run(dfa, input));
}