# NFA to DFA conversion

Bharadwaj Raju, University of Hyderabad.

## Structure

`fa.h`, `dfa.h`, `nfa.h` provide structures and functions for creating, running, and converting between NFAs and DFAs. `set.h` provides an array-based set structure which can map string keys to arbitrary values.

`nfa-conv` is a program that takes an input NFA and converts it into a DFA, and then lets you test strings against it.

Some sample FAs are provided:
  - `evenodd.dfa`: accepts if numbers of 'a' and 'b' in input are both odd or both even
  - `zeroonetwo.nfa`: accepts strings made of any number of `0`s followed by any number of `1`s followed by any number of `2`s
  - `realnumber.nfa`: accepts a real number, like `-.5`, `+1.5E-3`, `.1E2`, `-100` etc.

## Building and Using

```console
$ make
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