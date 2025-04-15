#ifndef LIBCHIMP_FILE_ITERATOR_H
#define LIBCHIMP_FILE_ITERATOR_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct File_Iterator_Position File_Iterator_Position;
struct File_Iterator_Position {
    uint64_t offset;
    uint64_t line;
    uint64_t column;
};

typedef struct File_Iterator File_Iterator;
struct File_Iterator {
    FILE* const file;
    File_Iterator_Position position;
};

typedef struct File_Iterator_Result File_Iterator_Result;
struct File_Iterator_Result {
    File_Iterator_Position position;
    int byte;
};

// Create a file iterator.
__attribute__((warn_unused_result))
File_Iterator file_iterator_create(FILE* const file) {
    assert(file != NULL);
    return (File_Iterator) {
        .file = file,
        .position = { .offset = 0, .line = 1, .column = 1 },
    };
}

// Read the next byte and increment the offset.
__attribute__((warn_unused_result))
File_Iterator_Result file_iterator_next(File_Iterator* const iter) {
    assert(iter != NULL);
    assert(iter->file != NULL);

    File_Iterator_Result result = {
        .position = iter->position,
        .byte = 0,
    };

    int const byte = fgetc(iter->file);
    if (byte != EOF) {
        result.byte = byte;
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
File_Iterator_Result file_iterator_peek(const File_Iterator* const iter) {
    assert(iter != NULL);
    assert(iter->file != NULL);

    File_Iterator_Result result = {
        .position = iter->position,
        .byte = 0,
    };

    char const byte = fgetc(iter->file);
    if (byte != EOF) {
        result.byte = byte;
        fseek(iter->file, -1, SEEK_CUR);
    }

    return result;
}

#endif
