#include <ctype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/stack.h"
#include "pda.h"

pda_t * pda_init()
{
    pda_t * pda = malloc(sizeof(pda_t));
    pda->initial[0] = '\0';
    pda->accepted = set_init();
    pda->transitions = set_init();
    return pda;
}

pda_t * pda_from_file(FILE * fp)
{
    pda_t * pda = pda_init();
    char buf[1024];
    int lineno = 1;
    while (fgets(buf, 1024, fp)) {
        char * buf_trim = buf;
        while (isspace(buf_trim[0])) {
            buf_trim++;
        }
        if (buf_trim[0] == '#' || buf_trim[0] == '\0') {
            continue;
        }
        char state1[128];
        char state2[128];
        char inputc;
        char stackc;
        char to_stackc;
        if (sscanf(buf_trim, "-> %s", state1) == 1) {
            pda_set_initial(pda, state1);
        } else if (sscanf(buf_trim, "%s (%c/%c) -> %s (%c)", state1, &inputc, &stackc, state2, &to_stackc) == 5) {
            pda_add_transition(pda, state1, inputc, stackc, state2, to_stackc);
        } else if (strncmp("A = ", buf_trim, 4) == 0) {
            char accepted_states_str[1024];
            if (sscanf(buf_trim, "A = [%[^]]]", accepted_states_str) != 1) {
                fprintf(stderr, "could not parse line %d\n", lineno);
                return NULL;
            }
            char * accepted_state;
            char * rest = accepted_states_str;
            while ((accepted_state = strtok_r(rest, " ,", &rest))) {
                pda_add_accepted(pda, accepted_state);
            }
        } else {
            fprintf(stderr, "could not parse line %d: '%s'\n", lineno, buf_trim);
            return NULL;
        }
        lineno++;
    }
    return pda;
}

void pda_to_file(pda_t * pda, FILE * fp)
{
    fprintf(fp, "-> %s\n", pda->initial);
    for (size_t i = 0; i < pda->transitions->len; i++) {
        char * lhs = pda->transitions->data[i].key;
        set_t * transitions_for = pda->transitions->data[i].data;
        for (size_t j = 0; j < transitions_for->len; j++) {
            char * rhs = transitions_for->data[j].key;
            fprintf(fp, "%s -> %s\n", lhs, rhs);
        }
    }
    fprintf(fp, "A = [");
    for (size_t i = 0; i < pda->accepted->len; i++) {
        fprintf(fp, "%s", pda->accepted->data[i].key);
        if (i != pda->accepted->len - 1) {
            fprintf(fp, " ");
        }
    }
    fprintf(fp, "]\n");
}

void pda_set_initial(pda_t * pda, const char * initial)
{
    strncpy(pda->initial, initial, KEY_LEN);
}

void pda_add_accepted(pda_t * pda, const char * accepted)
{
    set_add(pda->accepted, accepted, &PRESENT);
}

void pda_add_transition(pda_t * pda, const char * from_state, char inputc, char stackc, const char * to_state, char to_stackc)
{
    char * lhs = malloc(sizeof(char) * KEY_LEN + 4);
    snprintf(lhs, KEY_LEN, "%s (%c/%c)", from_state, inputc, stackc);
    char * rhs = malloc(sizeof(char) * KEY_LEN + 4);
    snprintf(rhs, KEY_LEN, "%s (%c)", to_state, to_stackc);
    set_t * transitions_for = set_find(pda->transitions, lhs);
    if (!transitions_for) {
        transitions_for = set_init();
        set_add(pda->transitions, lhs, transitions_for);
    }
    set_add(transitions_for, rhs, &PRESENT);
}

pda_branch_t * pda_branch_create(const char * state, size_t input_pos, stack_t * stack)
{
    pda_branch_t * branch = malloc(sizeof(pda_branch_t));
    strncpy(branch->state, state, KEY_LEN);
    if (stack == NULL) {
        stack = stack_init();
    }
    branch->stack = stack;
    branch->input_pos = input_pos;
    return branch;
}

void pda_branch(pda_t * pda, set_t * next_branches, pda_branch_t * branch, char inputc, char stackc)
{
    char * lhs = malloc(sizeof(char) * KEY_LEN + 6);
    snprintf(lhs, KEY_LEN + 6, "%s (%c/%c)", branch->state, inputc, stackc);
    set_t * transitions_for = set_find(pda->transitions, lhs);
    stack_t * stack = stack_copy(branch->stack);
    if (stackc != EPSILON) {
        stack_pop(stack);
    }
    if (transitions_for && transitions_for->len > 0) {
        for (size_t j = 0; j < transitions_for->len; j++) {
            char * rhs = transitions_for->data[j].key;
            char * to_state = malloc(sizeof(char) * KEY_LEN);
            char next_stackc;
            sscanf(rhs, "%s (%c)", to_state, &next_stackc);
            if (next_stackc != EPSILON) {
                stack_push(stack, next_stackc);
            }
            int input_pos = branch->input_pos;
            if (inputc != EPSILON) {
                input_pos++;
            }
            set_add(next_branches, "", pda_branch_create(to_state, input_pos, stack));
        }
    }
    free(lhs);
}

bool pda_recognize(pda_t * pda, const char * input)
{
    set_t * branches = set_init();
    set_add(branches, "0", pda_branch_create(pda->initial, 0, NULL));
    size_t input_len = strlen(input);
    size_t steps = 0;
    while (true) {
        steps++;
        set_t * next_branches = set_init();
        for (size_t i = 0; i < branches->len; i++) {
            pda_branch_t * branch = branches->data[i].data;
            char inputc;
            if (branch->input_pos >= input_len) {
                if (set_find(pda->accepted, branch->state) != NULL) {
                    stack_free(branch->stack);
                    set_free(branches);
                    set_free(next_branches);
                    printf("steps: %lu\n", steps);
                    return true;
                }
                inputc = EPSILON;
            } else {
                inputc = input[branch->input_pos];
            }
            char stackc = stack_peek(branch->stack);
            if (stackc != '\0') {
                pda_branch(pda, next_branches, branch, inputc, stackc);
                pda_branch(pda, next_branches, branch, EPSILON, stackc);
            }
            pda_branch(pda, next_branches, branch, inputc, EPSILON);
            pda_branch(pda, next_branches, branch, EPSILON, EPSILON);
        }
        set_free(branches);
        branches = next_branches;
        next_branches = set_init();
        if (branches->len == 0) {
            printf("steps: %lu\n", steps);
            break;
        }
    }
    return false;
}