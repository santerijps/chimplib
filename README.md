# libchimp

A collection of minimal, yet powerful functions and macros to help with most usual cases in C programming.

## Features

Currently, `libchimp` features:

- Better asserts
	- Nice output when run in tests (with `-DTEST` compiler flag)
- Assumes (soft asserts, return instead of crashing)
- `errorf`, `panicf`, `unreachable` macros and more!
- Shorthand types (`i32`, `f64`, etc.)
- Arena allocator
- String builder

## Compilation flags

It's recommended to compile your programs with the following flags:

```Makefile
test:
	gcc -O2 -Wall -Wextra -DTEST
debug:
	gcc -Og -Wall -Wextra
release:
	gcc -O3 -Wall -Wextra -Werror -s -DNDEBUG
```
