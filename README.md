# libchimp

A collection of minimal, yet powerful functions and macros to help with most usual cases in C programming.

## Features

Currently, `libchimp` features:

- Better asserts
	- Nicer output when run in tests (with `-DTEST` compiler flag)
- `assume` and `assumef` (soft assert, return instead of crashing)
- `errorf`, `panicf`, `unreachable` macros and more!
- Shorthand types (`i32`, `f64`, etc.)
- Arena allocator
- String builder

## Design decisions

- No heap allocations
- Lots of asserts
- Macros are kept minimal

## Usage

There are a few ways you might want to use this library:

- Copy-paste the code, there are no dependencies!
- Download the file you need and then include it in your project
- In your project directory:
	1. `git submodule add https://github.com/santerijps/chimplib`
	2. Compile with the flag: `-Ichimplib`
	3. Include in the source code: `#include <chimp/types.h>`

## Recommendations

### Language server

It's a good idea to utilize a language server in development.
A great option for this is [`clangd`](https://clangd.llvm.org/),
which is also available as a vscode extension.

Change `clangd` features by having a `.clangd` file in the root of your project:

```yaml
CompileFlags:
    Add:
        - "-Ichimplib" # Add includes
Diagnostics:
    Suppress:
		- some clangd warning # Suppress warnings
```

### Compilation flags

It's recommended to compile your programs with the following flags:

```Makefile
test:
	gcc -O2 -Wall -Wextra -DTEST
debug:
	gcc -Og -Wall -Wextra
release:
	gcc -O3 -Wall -Wextra -Werror -s -DNDEBUG
```

## TODO

Remember to keep the scope small!

- File content iterator (no memory allocations)
- Tools for dealing with `system` and `popen` to run external programs and read their output
- Header file for testing macros
