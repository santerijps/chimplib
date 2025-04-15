#ifndef LIBCHIMP_FILLER_H
#define LIBCHIMP_FILLER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define fallthrough __attribute__((fallthrough))
#define errorf(...) fprintf(stderr, ##__VA_ARGS__)
#define exitf(code, ...) do { errorf(__VA_ARGS__); exit(code); } while (0)
#define panicf(...) exitf(1, __VA_ARGS__)
#define unreachable panicf("Uncreachable code detected\n")

__attribute__((warn_unused_result))
size_t fsize(FILE* const file) {
    size_t const offset = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t const size = ftell(file);
    fseek(file, offset, SEEK_SET);
    return size;
}

#endif
