#include <ctype.h>
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

fa_t * fa_from_file(FILE * fp)
{
    fa_t * fa = fa_init();
    char buf[1024];
    int lineno = 1;
    while (fgets(buf, 1024, fp)) {
        char * buf_trim = buf;
        while (isspace(buf_trim[0])) {
            buf_trim++;
        }
        if (buf_trim[0] == '#') {
            continue;
        }
        char state1[128];
        char state2[128];
        char trans;
        if (sscanf(buf_trim, "-> %s", state1) == 1) {
            fa_set_initial(fa, state1);
        } else if (sscanf(buf_trim, "%s -> %s", state1, state2) == 2) {
            fa_add_lambda_transition(fa, state1, state2);
        } else if (sscanf(buf_trim, "%s (\\d) -> %s", state1, state2) == 2) {
            for (char trans = '0'; trans <= '9'; trans++) {
                fa_add_transition(fa, state1, trans, state2);
            }
        } else if (sscanf(buf_trim, "%s (\\l) -> %s", state1, state2) == 2) {
            for (char trans = 'a'; trans <= 'z'; trans++) {
                fa_add_transition(fa, state1, trans, state2);
            }
            for (char trans = 'A'; trans <= 'Z'; trans++) {
                fa_add_transition(fa, state1, trans, state2);
            }
        } else if (sscanf(buf_trim, "%s (%c) -> %s", state1, &trans, state2) == 3) {
            fa_add_transition(fa, state1, trans, state2);
        } else if (strncmp("A = ", buf_trim, 4) == 0) {
            char accepted_states_str[1024];
            if (sscanf(buf_trim, "A = [%[^]]]", accepted_states_str) != 1) {
                fprintf(stderr, "could not parse line %d\n", lineno);
                return NULL;
            }
            char * accepted_state;
            char * rest = accepted_states_str;
            while ((accepted_state = strtok_r(rest, " ,", &rest))) {
                fa_add_accepted(fa, accepted_state);
            }
        }
    }
    return fa;
}

void fa_to_file(fa_t * fa, FILE * fp)
{
    fprintf(fp, "-> %s\n", fa->initial);
    for (size_t i = 0; i < fa->transitions->len; i++) {
        char * from = fa->transitions->data[i].key;
        set_t * transitions = fa->transitions->data[i].data;
        for (size_t j = 0; j < transitions->len; j++) {
            char trans_char = transitions->data[j].key[0];
            set_t * char_transitions = transitions->data[j].data;
            for (size_t k = 0; k < char_transitions->len; k++) {
                char * to = char_transitions->data[k].key;
                fprintf(fp, "%s (%c) -> %s\n", from, trans_char, to);
            }
        }
    }
    for (size_t i = 0; i < fa->lambda_transitions->len; i++) {
        char * from = fa->lambda_transitions->data[i].key;
        set_t * transitions = fa->lambda_transitions->data[i].data;
        for (size_t j = 0; j < transitions->len; j++) {
            char * to = transitions->data[j].key;
            fprintf(fp, "%s -> %s\n", to, from);
        }
    }
    fprintf(fp, "A = [");
    for (size_t i = 0; i < fa->accepted->len; i++) {
        fprintf(fp, "%s", fa->accepted->data[i].key);
        if (i != fa->accepted->len - 1) {
            fprintf(fp, " ");
        }
    }
    fprintf(fp, "]\n");
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