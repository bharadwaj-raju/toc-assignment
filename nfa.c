#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nfa.h"

void extend_by_lambda_closure(fa_t * nfa, set_t * states)
{
    unsigned int done = 0;
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

    return reachable_states;
}

fa_t * nfa_to_dfa(fa_t * nfa)
{
    fa_t * dfa = fa_init();
    set_t * initial = set_init();
    set_add(initial, nfa->initial, &PRESENT);
    extend_by_lambda_closure(nfa, initial);
    fa_set_initial(dfa, "S0");
    if (set_intersection(initial, nfa->accepted)->len != 0) {
        fa_add_accepted(dfa, "S0");
    }
    set_t * unique_states = set_init();
    set_add(unique_states, "S0", initial);
    set_t * symbols = set_init();
    for (size_t i = 0; i < nfa->transitions->len; i++) {
        set_t * transitions = nfa->transitions->data[i].data;
        set_union_inplace(symbols, transitions);
        // for (size_t j = 0; j < transitions->len; j++) {
        //     char * trans_char = transitions->data[j].key;
        //     set_add_uniq(symbols, trans_char, &PRESENT);
        // }
    }
    for (size_t done = 0; done < unique_states->len; done++) {
        char * curr_stateset_name = unique_states->data[done].key;
        set_t * curr_stateset = unique_states->data[done].data;
        for (size_t i = 0; i < symbols->len; i++) {
            char * trans_str = symbols->data[i].key;
            set_t * reachable_from_curr = set_init();
            for (size_t j = 0; j < curr_stateset->len; j++) {
                char * state = curr_stateset->data[j].key;
                set_t * transitions = set_find(nfa->transitions, state);
                if (!transitions) {
                    continue;
                }
                set_t * char_transitions = set_find(transitions, trans_str);
                if (!char_transitions) {
                    continue;
                }
                set_union_inplace(reachable_from_curr, char_transitions);
                extend_by_lambda_closure(nfa, reachable_from_curr);
            }
            char * reached_stateset_name = set_find_by_data(unique_states, (find_by_data_fn *)set_equal, reachable_from_curr);
            if (reached_stateset_name == NULL) {
                // new state discovered
                reached_stateset_name = malloc(sizeof(char) * KEY_LEN);
                snprintf(reached_stateset_name, KEY_LEN, "S%d", unique_states->len);
                set_add(unique_states, reached_stateset_name, reachable_from_curr);
                if (set_intersection(reachable_from_curr, nfa->accepted)->len != 0) {
                    fa_add_accepted(dfa, reached_stateset_name);
                }
            } else {
                // just an old state -- we can safely free it
                set_clear(reachable_from_curr);
                free(reachable_from_curr);
            }
            fa_add_transition(dfa, curr_stateset_name, trans_str[0], reached_stateset_name);
        }
    }

    return dfa;
}