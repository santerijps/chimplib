MAKEFLAGS += --silent
CFLAGS := gcc -Og -Wall -Wextra -Werror -DTEST -DNDEBUG

test_all: build_all
	./string_builder_test

build_all:
	$(CFLAGS) -o string_builder_test.exe string_builder_test.c
