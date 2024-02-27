#ifndef NFA_H
#define NFA_H

#include "fa.h"

set_t * nfa_run(fa_t * nfa, const char * input);
void extend_by_lambda_closure(fa_t * nfa, set_t * states);

#endif