#ifndef CFG_H
#define CFG_H

#include <stdbool.h>
#include <stdio.h>

#include "set.h"

typedef struct cfg
{
    // set of productions
    set_t * productions;
    // start variable
    char start[KEY_LEN];
} cfg_t;

cfg_t * cfg_init();
cfg_t * cfg_from_file(FILE * fp);
void cfg_to_file(cfg_t * cfg, FILE * fp);
void cfg_add_production(cfg_t * cfg, const char * variable, const char * substitution);
void cfg_set_start_variable(cfg_t * cfg, const char * start_variable);
bool cfg_is_variable(cfg_t * cfg, char sym);
bool cfg_is_nullable(cfg_t * cfg, char sym);
void cfg_remove_nongenerating(cfg_t * cfg);
void cfg_remove_unreachable(cfg_t * cfg);
void cfg_purify(cfg_t * cfg);
bool cfg_is_chomsky_normalized(cfg_t * cfg);
void cfg_chomsky_normalize(cfg_t * cfg);

#endif