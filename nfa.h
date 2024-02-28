#ifndef NFA_H
#define NFA_H

#include "fa.h"

extern int PRESENT;

set_t * nfa_run(fa_t * nfa, const char * input);
void extend_by_lambda_closure(fa_t * nfa, set_t * states);
fa_t * nfa_to_dfa(fa_t * nfa);

#endif