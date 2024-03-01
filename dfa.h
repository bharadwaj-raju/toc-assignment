#ifndef DFA_H
#define DFA_H

#include <stdbool.h>

#include "fa.h"

extern int PRESENT;

char * dfa_step(fa_t * dfa, char * state, char c);
char * dfa_run(fa_t * dfa, const char * input);
bool dfa_accepts(fa_t * dfa, const char * input);

#endif