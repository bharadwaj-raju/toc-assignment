CFLAGS=-Wall -Wextra -Werror -ggdb3 -lm

nfa-conv: nfa-conv.c set.o fa.o nfa.o dfa.o
set.o: set.c set.h
fa.o: fa.c fa.h
nfa.o: nfa.c nfa.h
dfa.o: dfa.c dfa.h
