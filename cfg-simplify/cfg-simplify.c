#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cfg.h"

int main(int argc, char const * argv[])
{
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        printf("\nFile format:\n\n");
        printf("# production\n");
        printf("S -> ABC\n");
        printf("# null production\n");
        printf("S -> ''\n");
        return 1;
    }

    const char * fname = argv[1];
    FILE * fp = fopen(fname, "r");
    cfg_t * cfg = cfg_from_file(fp);
    fclose(fp);

    if (cfg == NULL) {
        return 1;
    }

    cfg_purify(cfg);

    printf("simplified:\n");
    cfg_to_file(cfg, stdout);

    return 0;
}
