# Pushdown Automata

Bharadwaj Raju, University of Hyderabad

## Structure

`pda.h` provides structures and functions for creating and running PDAs. `stack.h` provides an array-based stack structure.

`pda-run` is a program that takes an input PDA and lets you test strings against it. You can use the option `--verbose`/`-v` to make it print the exact process of the PDA.

Some sample PDAs are provided:
  - `anbn.pda`: accepts a<sup>n</sup>b<sup>n</sup>
  - `anbmambn.pda`: accepts a<sup>n</sup>b<sup>m</sup>a<sup>m</sup>b<sup>n</sup>
  - `anbmanplusm.pda`: accepts a<sup>n</sup>b<sup>m</sup>a<sup>n+m</sup>
  - `aibjck.pda`: accepts a<sup>i</sup>b<sup>j</sup>c<sup>k</sup> such that i, j, k ≥ 0 and i = j or i = k

## Building and Using

```console
$ make
$ ./pda-run
Usage: ./pda-run <file> [--verbose/-v]

File format:

# initial state
-> q1
# transition from q1 to q2 on reading a from input and b from stack, pushing c
# use space for epsilon
q1 (a/b) -> q2 (c)
# list accepted states
A = [state1 state2 stateN]
```