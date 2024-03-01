CFLAGS=-Wall -Wextra -Werror -ggdb3 -lm

all:
	$(CC) $(CFLAGS) nfa-conv.c set.c fa.c dfa.c nfa.c -o nfa-conv
