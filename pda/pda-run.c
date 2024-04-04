#include <stdio.h>
#include <string.h>

#include "pda.h"

int main(int argc, char const * argv[])
{
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        printf("\nFile format:\n\n");
        printf("# initial state\n");
        printf("-> q1\n");
        printf("# transition from q1 to q2 on reading a from input and b from stack, pushing c\n");
        printf("# use space for epsilon\n");
        printf("q1 (a/b) -> q2 (c)\n");
        printf("# list accepted states\n");
        printf("A = [state1 state2 stateN]\n");
        return 1;
    }

    bool verbose = false;

    if (argc == 3) {
        if (strncmp(argv[2], "--verbose", 9) == 0 || strncmp(argv[2], "-v", 2) == 0) {
            verbose = true;
        }
    }

    const char * fname = argv[1];
    FILE * fp = fopen(fname, "r");
    pda_t * pda = pda_from_file(fp);
    fclose(fp);

    if (pda == NULL) {
        return 1;
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
        bool accepted = pda_recognize(pda, input, verbose);
        printf("%s\n", accepted ? "accepted" : "rejected");
    }

    return 0;
}