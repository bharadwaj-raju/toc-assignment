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

    const char * fname = argv[1];
    FILE * fp = fopen(fname, "r");
    fa_t * fa = fa_from_file(fp);
    fclose(fp);

    if (fa == NULL) {
        return 1;
    }

    fa_t * dfa = nfa_to_dfa(fa);
    printf("converted dfa:\n");
    fa_to_file(dfa, stdout);

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
        set_t * nfa_state = nfa_run(fa, input);
        printf("nfa:\n");
        printf("\tfinal state: ");
        set_print(nfa_state);
        set_t * accepted_intersection = set_intersection(fa->accepted, nfa_state);
        bool nfa_accepted = accepted_intersection->len != 0;
        set_clear(accepted_intersection);
        free(accepted_intersection);
        printf("\taccepted: %s\n", nfa_accepted ? "yes" : "no");
        printf("dfa:\n");
        char * dfa_state = dfa_run(dfa, input);
        bool dfa_accepted = set_find(dfa->accepted, dfa_state) != NULL;
        printf("\tfinal state: %s\n", dfa_state);
        printf("\taccepted: %s\n", dfa_accepted ? "yes" : "no");
    }

    return 0;
}
