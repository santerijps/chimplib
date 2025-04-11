#ifndef LIBCHIMP_BYTE_ITERATOR_H
#define LIBCHIMP_BYTE_ITERATOR_H

#include <assert.h>
#include <stdint.h>

typedef struct Byte_Iterator_Position Byte_Iterator_Position;
struct Byte_Iterator_Position {
    uint64_t offset;
    uint64_t line;
    uint64_t column;
};

typedef struct Byte_Iterator Byte_Iterator;
struct Byte_Iterator {
    char* const string;
    size_t const length;
    uint64_t offset;
    Byte_Iterator_Position position;
};

typedef struct Byte_Iterator_Result Byte_Iterator_Result;
struct Byte_Iterator_Result {
    Byte_Iterator_Position position;
    int byte;
};

// Create a byte iterator.
__attribute__((warn_unused_result))
Byte_Iterator byte_iterator_create(
    char* const string,
    size_t const length
) {
    assert(string != NULL);
    assert(length > 0);
    return (Byte_Iterator) {
        .string = string,
        .length = length,
        .offset = 0,
        .position = { .offset = 0, .line = 1, .column = 1 },
    };
}

// Read the next byte and increment the offset.
__attribute__((warn_unused_result))
Byte_Iterator_Result byte_iterator_next(Byte_Iterator* const iter) {
    assert(iter != NULL);
    assert(iter->string != NULL);
    assert(iter->length > 0);
    assert(iter->offset <= iter->length);

    Byte_Iterator_Result result = {
        .position = iter->position,
        .byte = 0,
    };

    if (iter->offset < iter->length) {
        result.byte = iter->string[iter->offset];
        iter->offset += 1;
        iter->position.offset += 1;

        if (result.byte == '\n') {
            iter->position.line += 1;
            iter->position.column = 1;
        } else {
            iter->position.column += 1;
        }
    }

    return result;
}

// Read the next byte but DO NOT increment the offset.
__attribute__((warn_unused_result))
Byte_Iterator_Result byte_iterator_peek(const Byte_Iterator* const iter) {
    assert(iter != NULL);
    assert(iter->string != NULL);
    assert(iter->length > 0);
    assert(iter->offset <= iter->length);

    Byte_Iterator_Result result = {
        .position = iter->position,
        .byte = 0,
    };

    if (iter->offset < iter->length) {
        result.byte = iter->string[iter->offset];
    }

    return result;
}

#endif
