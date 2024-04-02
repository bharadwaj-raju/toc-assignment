# Context-Free Grammars: Simplification and Conversion to Chomsky Normal Form

Bharadwaj Raju, University of Hyderabad.

## Structure

`cfg.h` provide structures and functions for creating and purifying CFGs.

`cfg-simplify` is a program that takes an input CFG and purifies it, then converts it into CNF.

Some sample CFGs are provided.

## Building and Using

```console
$ make
$ ./cfg-simplify
Usage: ./cfg-simplify <file>

File format:

# production
S -> ABC
# null production
S -> ''
```