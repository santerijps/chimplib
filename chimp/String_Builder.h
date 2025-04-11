#ifndef LIBCHIMP_STRING_BUILDER_H
#define LIBCHIMP_STRING_BUILDER_H

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct String_Builder String_Builder;
struct String_Builder {
    char* const buffer;
    size_t const capacity;
    size_t length;
};

typedef enum String_Builder_Error {
    STRING_BUILDER_ERROR_NONE,
    STRING_BUILDER_ERROR_SOME,
} String_Builder_Error;

__attribute__((warn_unused_result))
String_Builder string_builder_create(
    char* const buffer,
    size_t const capacity
) {
    assert(buffer != NULL);
    assert(capacity > 0);
    memset(buffer, 0, capacity);

    return (String_Builder) {
        .buffer = buffer,
        .capacity = capacity,
        .length = 0,
    };
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_write_byte(
    String_Builder* const builder,
    char const byte
) {
    assert(builder != NULL);
    assert(builder->buffer != NULL);
    assert(builder->length < builder->capacity);
    assert(byte > 0);

    if (builder->length + 1 >= builder->capacity) {
        return STRING_BUILDER_ERROR_SOME;
    }

    builder->buffer[builder->length] = byte;
    builder->length += 1;

    return STRING_BUILDER_ERROR_NONE;
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_write_bytes(
    String_Builder* const builder,
    char* const bytes,
    size_t const count
) {
    assert(builder != NULL);
    assert(builder->buffer != NULL);
    assert(builder->length < builder->capacity);
    assert(bytes != NULL);

    size_t const buffer_length = builder->length;
    size_t const required_capacity = buffer_length + count;

    if (required_capacity >= builder->capacity) {
        return STRING_BUILDER_ERROR_SOME;
    }

    for (size_t i = 0; i < count; i += 1) {
        int const buffer_index = buffer_length + i;
        builder->buffer[buffer_index] = bytes[i];
    }

    builder->length += count;
    return STRING_BUILDER_ERROR_NONE;
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_write_string(
    String_Builder* const builder,
    char* const string
) {
    size_t const string_length = strlen(string);
    return string_builder_write_bytes(builder, string, string_length);
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_write_int(
    String_Builder* const builder,
    int64_t const value
) {
    assert(builder != NULL);
    assert(builder->buffer != NULL);
    assert(builder->length < builder->capacity);

    if (-1 < value && value < 10) {
        return string_builder_write_byte(builder, '0' + value);
    }

    if (-10 < value && value < 0) {
        if (string_builder_write_byte(builder, '-')) {
            return STRING_BUILDER_ERROR_SOME;
        }
        return string_builder_write_byte(builder, '0' + value);
    }

    int64_t const sign = value < 0 ? -1 : 1;
    uint64_t tmp = sign == -1 ? -value : value;

    char buffer [20] = {0};
    uint8_t length = 0;

    while (tmp > 0) {
        uint8_t modulo = tmp % 10;
        tmp /= 10;
        buffer[length] = '0' + modulo;
        length += 1;
    }

    if (sign == -1) {
        if (string_builder_write_byte(builder, '-')) {
            return STRING_BUILDER_ERROR_SOME;
        }
    }

    for (uint8_t i = 0; i < length; i += 1) {
        if (string_builder_write_byte(builder, buffer[length - i - 1])) {
            return STRING_BUILDER_ERROR_SOME;
        }
    }

    return STRING_BUILDER_ERROR_NONE;
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_write_uint(
    String_Builder* const builder,
    uint64_t const value
) {
    assert(builder != NULL);
    assert(builder->buffer != NULL);
    assert(builder->length < builder->capacity);

    if (value < 10) {
        return string_builder_write_byte(builder, '0' + value);
    }

    uint64_t tmp = value;
    char buffer [21] = {0};
    uint8_t length = 0;

    while (tmp > 0) {
        uint8_t modulo = tmp % 10;
        tmp /= 10;
        buffer[length] = '0' + modulo;
        length += 1;
    }

    for (uint8_t i = 0; i < length; i += 1) {
        if (string_builder_write_byte(builder, buffer[length - i - 1])) {
            return STRING_BUILDER_ERROR_SOME;
        }
    }

    return STRING_BUILDER_ERROR_NONE;
}

__attribute__((warn_unused_result))
String_Builder_Error string_builder_printf(
    String_Builder* const builder,
    char* const format_string,
    ...
) {
    assert(builder != NULL);
    assert(builder->buffer != NULL);
    assert(builder->length < builder->capacity);

    va_list var_args;
    va_start(var_args, format_string);

    for (int i = 0; format_string[i] != 0; i += 1) {

        if (format_string[i] != '%') {
            if (string_builder_write_byte(builder, format_string[i])) {
                return STRING_BUILDER_ERROR_SOME;
            }
            continue;
        }

        i += 1;
        if (format_string[i] == 0) {
            if (string_builder_write_byte(builder, '%')) {
                return STRING_BUILDER_ERROR_SOME;
            }
            break;
        }

        switch (format_string[i]) {

            case 'd':
            case 'i': {
                int64_t const value = va_arg(var_args, int64_t);
                if (string_builder_write_int(builder, value)) {
                    return STRING_BUILDER_ERROR_SOME;
                }
                break;
            }

            case 'u': {
                uint64_t const value = va_arg(var_args, uint64_t);
                if (string_builder_write_uint(builder, value)) {
                    return STRING_BUILDER_ERROR_SOME;
                }
                break;
            }

            case 'c': {
                char const value = va_arg(var_args, int);
                if (string_builder_write_byte(builder, value)) {
                    return STRING_BUILDER_ERROR_SOME;
                }
                break;
            }

            case 's': {
                char* const value = va_arg(var_args, char*);
                if (string_builder_write_string(builder, value)) {
                    return STRING_BUILDER_ERROR_SOME;
                }
                break;
            }

            default: {
                char buffer[] = { '%', format_string[i] };
                if (string_builder_write_bytes(builder, buffer, sizeof(buffer))) {
                    return STRING_BUILDER_ERROR_SOME;
                }
            }
        } // switch
    } // for

    return STRING_BUILDER_ERROR_NONE;
}

#endif

