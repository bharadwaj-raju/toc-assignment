# NFA-λ to DFA conversion

Bharadwaj Raju, University of Hyderabad.

## Structure

`fa.h`, `dfa.h`, `nfa.h` provide structures and functions for creating, running, and converting between NFA-λs and DFAs. `set.h` provides an array-based set structure which can map string keys to arbitrary values.

`nfa-conv` is a program that takes an input NFA-λ and converts it into a DFA, and then lets you test strings against it.  The option `--verbose`/`-v` can be used to make it print the exact working process.

Some sample FAs are provided:
  - `evenodd.dfa`: accepts if numbers of 'a' and 'b' in input are both odd or both even
  - `zeroonetwo.nfa`: accepts strings made of any number of `0`s followed by any number of `1`s followed by any number of `2`s
  - `realnumber.nfa`: accepts a real number, like `-.5`, `+1.5E-3`, `.1E2`, `-100` etc.
  - `identifier.nfa`: accepts valid C identifiers, i.e. matching regex `(l + _)(l + d + _)*`
  - `identifier-regexlike.nfa`: like `identifier.nfa`, but written as if it was mechanically converted to an NFA-λ from the regex. involves more states.

## Building and Using

```console
$ make
$ ./nfa-conv
Usage: ./nfa-conv <file> [--verbose/-v]

File format:

# indicate initial state
-> state0
# transition on character 'c' from state1 to state2
state1 (c) -> state2
# lambda transition from state1 to state2
state1 -> state2
# shortcut to define transition on all digits (0-9)
state1 (\d) -> state2
# shortcut to define transition on all letters (a-z and A-Z)
state1 (\l) -> state2
# list accepted states
A = [state1 state2 stateN]
```