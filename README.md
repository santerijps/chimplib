# libchimp

A collection of minimal, yet powerful functions and macros to help with most usual cases in C programming.

## Features

Currently, `libchimp` features:

- Better asserts
- Testing utilities
- `assume` and `assumef` (soft assert, return instead of crashing)
- `eprintf`, `panicf`, `unreachable` macros and more!
- Shorthand types (`i32`, `f64`, etc.)
- Arena allocator
- String builder
- More!

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

## Documentation

Learn how to use the functions by reading their source code or by [looking into the tests cases](./tests/)

## Recommendations

### Language server

It's a good idea to utilize a language server in development.
A great option for this is [`clangd`](https://clangd.llvm.org/),
which is also available as a vscode extension.

Change `clangd` features by having a `.clangd` file in the root of your project:

```yaml
CompileFlags:
    Add:
        - "-Ichimplib" # Add chimplib to search path
		- "-xc" # Language is C
        - "-std=c99" # C standard to adhere to
        - "-Wpedantic" # Warn in case non-standard features are used
Diagnostics:
    Suppress:
        - misc-definitions-in-headers # Suppress warnings
```

### Compilation flags

It's recommended to compile your programs with the following flags:

```Makefile
# Example Makefile
MAKEFLAGS += --silent

WARNINGS := -Wall -Wextra -Wshadow -Wformat=2 -Wnull-dereference -Wpedantic
SAFETY := -fstack-protector -D_FORTIFY_SOURCE=2 -fno-strict-aliasing
COMMON_FLAGS := $(WARNINGS) $(SAFETY) -std=c99
REDUCE_SIZE := -s -Wl,--gc-sections -fdata-sections -ffunction-sections

debug:
	gcc -Og -g $(COMMON_FLAGS)
release:
	gcc -O3 $(REDUCE_SIZE) $(COMMON_FLAGS) -Werror -flto -DNDEBUG
```

## Development

Below is the list of tools used in development:

- `gcc` [Home page](https://gcc.gnu.org/)
- `clangd` [Home page](https://clangd.llvm.org/)

## TODO

Remember to keep the scope small!

- Fix pedantic errors
- File content iterator (no memory allocations)
- Tools for dealing with `system` and `popen` to run external programs and read their output
