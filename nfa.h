#include <stdlib.h>
#include <string.h>

#ifndef NFA_H
#define NFA_H

#include "fa.h"
#include "set.h"

set_t * nfa_run(fa_t * nfa, const char * input)
{
    set_t * reachable_states = set_init();
    set_add(reachable_states, nfa->initial, &PRESENT);
    extend_by_lambda_closure(nfa, reachable_states);
    set_t * next_states = set_init();
    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        char * c_str = single_char_str(c);
        for (size_t i = 0; i < reachable_states->len; i++) {
            char * state = reachable_states->data[i].key;
            set_t * transitions = set_find(nfa->transitions, state);
            if (!transitions) {
                continue;
            }
            set_t * char_transitions = set_find(transitions, c_str);
            if (!char_transitions) {
                continue;
            }
            set_union_inplace(next_states, char_transitions);
            extend_by_lambda_closure(nfa, next_states);
        }
        set_clear(reachable_states);
        free(reachable_states);
        reachable_states = next_states;
        next_states = set_init();
    }

    return next_states;
}

void extend_by_lambda_closure(fa_t * nfa, set_t * states)
{
    int done = 0;
    while (done < states->len) {
        for (size_t i = done; i < states->len; i++) {
            char * state = states->data[i].key;
            set_t * lambda_transitions = set_find(nfa->lambda_transitions, state);
            if (!lambda_transitions) {
                done++;
                continue;
            }
            set_union_inplace(states, lambda_transitions);
            done++;
        }
    }
}

#endif