# Pushdown Automata

Bharadwaj Raju, University of Hyderabad

## Structure

`pda.h` provides structures and functions for creating and running PDAs. `stack.h` provides an array-based stack structure.

`pda-run` is a program that takes an input PDA and lets you test strings against it.

Some sample PDAs are provided:
  - `anbn.pda`: accepts a^nb^n
  - `anbmambn.pda`: accepts a^nb^ma^mb^n
  - `anbmanplusm.pda`: accepts a^nb^ma^(n+m)

## Building and Using

```console
$ make
$ ./pda-run
Usage: ./pda-run <file>

File format:

# initial state
-> q1
# transition from q1 to q2 on reading a from input and b from stack, pushing c
# use space for epsilon
q1 (a/b) -> q2 (c)
# list accepted states
A = [state1 state2 stateN]
```