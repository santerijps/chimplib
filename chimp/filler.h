#ifndef LIBCHIMP_FILLER_H
#define LIBCHIMP_FILLER_H

#include <assert.h>
#include <stdio.h>

size_t fsize(FILE* const file) {
    size_t const offset = ftell(file);
    assert(fseek(file, 0, SEEK_END) == 0);
    size_t const size = ftell(file);
    assert(fseek(file, offset, SEEK_SET) == 0);
    return size;
}

#endif
