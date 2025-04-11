MAKEFLAGS += --silent
CFLAGS := gcc -Og -Wall -Wextra -Werror

test_all: build_all_tests
	bin/string_builder_test

build_all_tests: bin
	$(CFLAGS) -o bin/string_builder_test.exe tests/string_builder_test.c

bin:
	mkdir bin
