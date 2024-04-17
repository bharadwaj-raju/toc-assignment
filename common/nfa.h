#ifndef NFA_H
#define NFA_H

#include "fa.h"

extern int PRESENT;

void nfa_set_verbose(bool v);
set_t * nfa_step(fa_t * nfa, set_t * state, char c);
set_t * nfa_run(fa_t * nfa, const char * input);
bool nfa_accepts(fa_t * nfa, const char * input);
void extend_by_lambda_closure(fa_t * nfa, set_t * states);
fa_t * nfa_to_dfa(fa_t * nfa);

#endif