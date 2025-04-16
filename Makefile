MAKEFLAGS += --silent

WARNINGS := -Wall -Wextra -Wshadow -Wformat=2 -Wnull-dereference -Wpedantic
SAFETY := -fstack-protector -D_FORTIFY_SOURCE=2 -fno-strict-aliasing
COMPILE := gcc $(WARNINGS) $(SAFETY) -Werror -Og -g

test_all: build_all_tests
	bin/string_builder_test

build_all_tests: bin
	$(COMPILE) -o bin/string_builder_test.exe tests/string_builder_test.c

bin:
	mkdir bin
