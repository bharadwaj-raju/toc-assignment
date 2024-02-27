#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

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
    }

    return 0;
}
