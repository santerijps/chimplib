#ifndef LIBCHIMP_FILE_READER_H
#define LIBCHIMP_FILE_READER_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct File_Reader File_Reader;
struct File_Reader {
    FILE* const file;
    char* buffer;
    size_t const buffer_size;
    size_t buffer_length;
    int64_t buffer_index;
    char is_eof;
};

// Create a file reader.
__attribute__((warn_unused_result))
File_Reader file_reader_create(
    FILE* const file,
    char* const buffer,
    size_t const buffer_size
) {
    assert(file != NULL);
    assert(buffer != NULL);
    assert(buffer_size > 0);
    return (File_Reader) {
        .file = file,
        .buffer = buffer,
        .buffer_size = buffer_size,
        .buffer_length = 0,
        .buffer_index = 0,
        .is_eof = 0,
    };
}

// Refresh the file reader buffer if needed.
// Return 0 if all is good.
// Return EOF if there are no more bytes to read.
__attribute__((warn_unused_result))
int file_reader_refresh(File_Reader* const reader) {
    assert(reader != NULL);
    assert(reader->file != NULL);
    assert(reader->buffer != NULL);
    assert(reader->buffer_size > 0);
    assert(reader->buffer_index <= reader->buffer_length);
    assert(reader->buffer_index <= reader->buffer_size);

    if (reader->is_eof) {
        return EOF;
    }

    if (reader->buffer_index >= reader->buffer_length) {
        reader->buffer_length = fread(reader->buffer, 1, reader->buffer_size, reader->file);
        reader->buffer_index = 0;

        if (reader->buffer_length == 0) {
            reader->is_eof = 1;
            return EOF;
        }
    }

    return 0;
}

// Read a single byte.
// Return the byte if all is good.
// Return EOF if no more bytes can be read.
__attribute__((warn_unused_result))
char file_reader_read_byte(File_Reader* const reader) {
    assert(reader != NULL);
    assert(reader->file != NULL);
    assert(reader->buffer != NULL);
    assert(reader->buffer_size > 0);
    assert(reader->buffer_index <= reader->buffer_length);
    assert(reader->buffer_index <= reader->buffer_size);

    if (file_reader_refresh(reader) == EOF) {
        return EOF;
    }

    char const byte = reader->buffer[reader->buffer_index];
    reader->buffer_index += 1;

    return byte;
}

// Read some bytes.
// Return a char* if all is good.
// Return NULL if no more bytes can be read.
__attribute__((warn_unused_result))
size_t file_reader_read_bytes(
    File_Reader* const reader,
    char* const buffer,
    size_t const buffer_size
) {
    assert(reader != NULL);
    assert(reader->file != NULL);
    assert(reader->buffer != NULL);
    assert(reader->buffer_size > 0);
    assert(reader->buffer_index <= reader->buffer_length);
    assert(reader->buffer_index <= reader->buffer_size);
    assert(buffer != NULL);
    assert(buffer_size > 0);

    if (file_reader_refresh(reader) == EOF) {
        return 0;
    }

    size_t n = 0;
    for (size_t i = 0; i < buffer_size; i += 1) {
        char const byte = file_reader_read_byte(reader);
        if (byte == EOF) {
            break;
        }

        buffer[i] = byte;
        n += 1;
    }

    return n;
}

// Set the position in the file.
// Return 0 if all is good.
// Return EOF if no more bytes can be read.
__attribute__((warn_unused_result))
int file_reader_seek(
    File_Reader* const reader,
    int64_t const offset,
    int64_t const origin
) {
    assert(reader != NULL);
    assert(reader->file != NULL);
    assert(reader->buffer != NULL);
    assert(reader->buffer_size > 0);
    assert(reader->buffer_index <= reader->buffer_length);
    assert(reader->buffer_index <= reader->buffer_size);
    assert(SEEK_SET <= origin && origin <= SEEK_END);
    assert(fseek(reader->file, offset, origin) == 0);

    reader->buffer_index = 0;
    reader->buffer_length = 0;
    reader->is_eof = 0;

    if (file_reader_refresh(reader) == EOF) {
        return EOF;
    }

    return 0;
}

// Peek a byte in the reader.
// Return the peeked char ir all is good.
// Return EOF if nothing can be peeked.
__attribute__((warn_unused_result))
char file_reader_peek(File_Reader* const reader) {
    assert(reader != NULL);
    assert(reader->file != NULL);
    assert(reader->buffer != NULL);
    assert(reader->buffer_size > 0);
    assert(reader->buffer_index <= reader->buffer_length);
    assert(reader->buffer_index <= reader->buffer_size);

    char const byte = file_reader_read_byte(reader);
    if (byte == EOF) {
        return EOF;
    }

    assert(file_reader_seek(reader, -1, SEEK_CUR) == 0);

    return byte;
}

#endif
