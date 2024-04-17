#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nfa.h"

static bool verbose = false;

void nfa_set_verbose(bool v)
{
    verbose = v;
}

#define log(msg, ...) \
    if (verbose)      \
    fprintf(stdout, msg, ##__VA_ARGS__)

// extends a set of states to its lambda closure
// i.e. all the states that can be reached from states in that set
// using only lambda transitions
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

set_t * nfa_step(fa_t * nfa, set_t * state, char c)
{
    set_t * curr_reachable = set_copy(state);
    extend_by_lambda_closure(nfa, curr_reachable);

    if (verbose) {
        log("λ(");
        set_print(state);
        log(") = ");
        set_print(curr_reachable);
        log("\n");
    }

    set_t * next_reachable = set_init_with_capacity(curr_reachable->len);
    char * c_str = single_char_str(c);
    for (size_t i = 0; i < curr_reachable->len; i++) {
        char * state = curr_reachable->data[i].key;
        set_t * transitions_for_state = set_find(nfa->transitions, state);
        if (!transitions_for_state) {
            continue;
        }
        set_t * transitions_for_char = set_find(transitions_for_state, c_str);
        if (!transitions_for_char) {
            continue;
        }
        set_union_inplace(next_reachable, transitions_for_char);
        if (verbose) {
            log("  transitions on %c = ", c);
            set_print(next_reachable);
            log("\n");
        }
        extend_by_lambda_closure(nfa, next_reachable);
        if (verbose) {
            log("  λ(transitions on %c) = ", c);
            set_print(next_reachable);
            log("\n");
        }
    }
    free(c_str);
    set_free(curr_reachable);
    return next_reachable;
}

set_t * nfa_run(fa_t * nfa, const char * input)
{
    set_t * curr_reachable = set_init();
    set_add(curr_reachable, nfa->initial, &PRESENT);
    extend_by_lambda_closure(nfa, curr_reachable);
    for (size_t i = 0; i < strlen(input); i++) {
        set_t * next_reachable = nfa_step(nfa, curr_reachable, input[i]);
        set_free(curr_reachable);
        curr_reachable = next_reachable;
    }
    return curr_reachable;
}

bool nfa_accepts(fa_t * nfa, const char * input)
{
    set_t * final_states = nfa_run(nfa, input);
    set_t * accepted_intersection = set_intersection(final_states, nfa->accepted);
    bool accepts = accepted_intersection->len != 0;
    set_free(final_states);
    set_free(accepted_intersection);
    return accepts;
}

fa_t * nfa_to_dfa(fa_t * nfa)
{
    fa_t * dfa = fa_init();
    set_t * initial = set_init();
    set_add(initial, nfa->initial, &PRESENT);
    extend_by_lambda_closure(nfa, initial);
    if (verbose) {
        log("λ(initial state) = S0 = ");
        set_print(initial);
        log("\n");
    }
    fa_set_initial(dfa, "S0");
    if (set_intersection(initial, nfa->accepted)->len != 0) {
        log("S0 is an acceptance state\n");
        fa_add_accepted(dfa, "S0");
    }

    // each unique "stateset" in the NFA becomes one state in the DFA
    set_t * unique_statesets = set_init();
    set_add(unique_statesets, "S0", initial);

    // we need to build the set of all symbols used in the NFA
    // because we don't explicitly track the set of symbols
    // when constructing the FA
    set_t * symbols = set_init();
    for (size_t i = 0; i < nfa->transitions->len; i++) {
        set_t * transitions = nfa->transitions->data[i].data;
        set_union_inplace(symbols, transitions);
    }

    // to generate the set of all statesets that the NFA can be in
    // we repeatedly pick a known stateset and see what stateset it leads to
    // when we give it all possible input symbols
    // we add that back to the set of known statesets if it is unique
    // this process is repeated until we have done this for every stateset
    // in this loop, unique_statesets->len keeps increasing as we discover more
    // reaching the end of this loop means that we have exhausted all possible statesets
    for (size_t done = 0; done < unique_statesets->len; done++) {
        char * curr_stateset_name = unique_statesets->data[done].key;
        set_t * curr_stateset = unique_statesets->data[done].data;
        for (size_t i = 0; i < symbols->len; i++) {
            char * trans_str = symbols->data[i].key;
            set_t * reachable_from_curr = set_init();
            for (size_t j = 0; j < curr_stateset->len; j++) {
                char * state = curr_stateset->data[j].key;
                set_t * transitions_for_state = set_find(nfa->transitions, state);
                if (!transitions_for_state) {
                    continue;
                }
                set_t * transitions_for_char = set_find(transitions_for_state, trans_str);
                if (!transitions_for_char) {
                    continue;
                }
                set_union_inplace(reachable_from_curr, transitions_for_char);
                extend_by_lambda_closure(nfa, reachable_from_curr);
            }
            char * reached_stateset_name = set_find_by_data(unique_statesets, (find_by_data_fn *)set_equal, reachable_from_curr);
            if (reached_stateset_name == NULL) {
                // new stateset discovered
                reached_stateset_name = malloc(sizeof(char) * KEY_LEN);
                snprintf(reached_stateset_name, KEY_LEN, "S%d", unique_statesets->len);
                if (verbose) {
                    log("found new state %s = ", reached_stateset_name);
                    set_print(reachable_from_curr);
                    log("\n");
                }
                set_add(unique_statesets, reached_stateset_name, reachable_from_curr);
                if (set_intersection(reachable_from_curr, nfa->accepted)->len != 0) {
                    fa_add_accepted(dfa, reached_stateset_name);
                }
            } else {
                // just an old stateset -- we can safely free it
                set_free(reachable_from_curr);
            }
            log("%s transition on %c to %s\n", curr_stateset_name, trans_str[0], reached_stateset_name);
            fa_add_transition(dfa, curr_stateset_name, trans_str[0], reached_stateset_name);
        }
    }

    return dfa;
}