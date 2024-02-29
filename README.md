# NFA to DFA conversion

Bharadwaj Raju, University of Hyderabad.

## Structure

`fa.h`, `dfa.h`, `nfa.h` provide structures and functions for creating, running, and converting between NFAs and DFAs. `set.h` provides an array-based set structure which can map string keys to arbitrary values.

`nfa-conv` is a program that takes an input NFA and converts it into a DFA, and then lets you test strings against it.

## Building and Using

```console
$ make nfa-conv
$ ./nfa-conv
Usage: ./nfa-conv <file>

File format:

# indicate initial state
-> state0
# transition on character 'c' from state1 to state2
state1 (c) -> state2
# lambda transition from state1 to state2
state1 -> state2
# list accepted states
A = [state1 state2 stateN]
```