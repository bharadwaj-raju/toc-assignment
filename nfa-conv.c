#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfa.h"
#include "fa.h"
#include "nfa.h"

int main(int argc, char const * argv[])
{
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        printf("\nFile format:\n\n");
        printf("# indicate initial state\n");
        printf("-> state0\n");
        printf("# transition on character 'c' from state1 to state2\n");
        printf("state1 (c) -> state2\n");
        printf("# lambda transition from state1 to state2\n");
        printf("state1 -> state2\n");
        printf("# list accepted states\n");
        printf("A = [state1 state2 stateN]\n");
        return 1;
    }

    fa_t * fa = fa_init();

    const char * fname = argv[1];
    FILE * fp = fopen(fname, "r");
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
        } else if (sscanf(buf_trim, "%s (%c) -> %s", state1, &trans, state2) == 3) {
            fa_add_transition(fa, state1, trans, state2);
        } else if (strncmp("A = ", buf_trim, 4) == 0) {
            char accepted_states_str[1024];
            if (sscanf(buf_trim, "A = [%[^]]]", accepted_states_str) != 1) {
                fprintf(stderr, "could not parse line %d\n", lineno);
                return 1;
            }
            char * accepted_state;
            char * rest = accepted_states_str;
            while ((accepted_state = strtok_r(rest, " ,", &rest))) {
                fa_add_accepted(fa, accepted_state);
            }
        }
    }

    printf("enter strings to test\n");
    while (true) {
        char input[1024];
        printf("> ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        char * newline = strchr(input, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }
        printf("%s\n", dfa_run(fa, input));
        set_print(nfa_run(fa, input));
    }

    return 0;
}
