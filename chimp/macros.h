#ifndef LIBCHIMP_MACROS_H
#define LIBCHIMP_MACROS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define var __auto_type
#define let const var

#define fallthrough __attribute__((fallthrough))

#define errorf(...) fprintf(stderr, ##__VA_ARGS__)
#define exitf(code, ...) do { errorf(__VA_ARGS__); exit(code); } while (0)
#define panicf(...) exitf(1, __VA_ARGS__)
#define unreachable panicf("Uncreachable code detected\n")

#define assume(expr) do if (!(expr)) return 1; while (0)
#define assumef(expr, ...) do if(!(expr)) { errorf(__VA_ARGS__); return 1; } while (0)
#define assume_or(expr, code) do if (!(expr)) return (code); while (0)

#ifndef NDEBUG

    #define assertf(expr, ...)                                                                      \
        do if (!(expr)) {                                                                           \
            errorf("Assertion failed: %s (%s:%d in %s)\n", #expr, __FILE__, __LINE__, __func__);    \
            errorf(__VA_ARGS__);                                                                    \
            exit(1);                                                                                \
        } while (0)

    #define assert_equal(actual, expected)\
        do {                                                                                \
            let a = (int64_t)(actual);                                                      \
            let b = (int64_t)(expected);                                                    \
            if (a != b) {                                                                   \
                errorf(                                                                     \
                    "Assertion failed: Expected %s to be %lld, got %lld (%s:%d in %s)\n",   \
                    #actual, b, a, __FILE__, __LINE__, __func__                             \
                );                                                                          \
                exit(1);                                                                    \
            }                                                                               \
        } while (0)

    #define assert_equal_string(actual, expected)\
        do {                                                                                    \
            let a = (char*)(actual);                                                            \
            let b = (char*)(expected);                                                          \
            if (strcmp(a, b) != 0) {                                                            \
                errorf(                                                                         \
                    "Assertion failed: Expected %s to be \"%s\", got \"%s\" (%s:%d in %s)\n",   \
                    #actual, b, a, __FILE__, __LINE__, __func__                                 \
                );                                                                              \
                exit(1);                                                                        \
            }                                                                                   \
        } while (0)

#endif

#ifdef TEST

    #undef assert
    #undef assertf
    #undef assert_equal
    #undef assert_equal_string

    #define _ASSERTF_FORMAT "%s:%d:%s \e[31mFAIL\e[0m Assertion failed: %s\n"
    #define _ASSERT_EQUAL_FORMAT "%s:%d:%s \e[31mFAIL\e[0m Expected %s to be %lld, got %lld\n"
    #define _ASSERT_EQUAL_STRING_FORMAT "%s:%d:%s \e[31mFAIL\e[0m Expected %s to be \"%s\", got \"%s\"\n"

    #define assert(expr) assertf(expr, "\r")

    #define assertf(expr, ...)                      \
        do if (!(expr)) {                           \
            errorf(                                 \
                _ASSERTF_FORMAT,                    \
                __FILE__, __LINE__, __func__, #expr \
            );                                      \
            errorf(__VA_ARGS__);                    \
            return 1;                               \
        } while (0)

    #define assert_equal(actual, expected)                      \
        do {                                                    \
            let a = (int64_t)(actual);                          \
            let b = (int64_t)(expected);                        \
            if (a != b) {                                       \
                errorf(                                         \
                    _ASSERT_EQUAL_FORMAT,                       \
                    __FILE__, __LINE__, __func__, #actual, b, a \
                );                                              \
                return 1;                                       \
            }                                                   \
        } while (0)

    #define assert_equal_string(actual, expected)               \
        do {                                                    \
            let a = (char*)(actual);                            \
            let b = (char*)(expected);                          \
            if (strcmp(a, b) != 0) {                            \
                errorf(                                         \
                    _ASSERT_EQUAL_STRING_FORMAT,                \
                    __FILE__, __LINE__, __func__, #actual, b, a \
                );                                              \
                return 1;                                       \
            }                                                   \
        } while (0)

#endif

#endif
