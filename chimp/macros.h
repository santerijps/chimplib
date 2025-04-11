#ifndef LIBCHIMP_MACROS_H
#define LIBCHIMP_MACROS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            __auto_type a = (int64_t)(actual);                                              \
            __auto_type b = (int64_t)(expected);                                            \
            if (a != b) {                                                                   \
                errorf(                                                                     \
                    "Assertion failed: Expected %s to be %lld, got %lld (%s:%d in %s)\n",   \
                    #actual, b, a, __FILE__, __LINE__, __func__                             \
                );                                                                          \
                exit(1);                                                                    \
            }                                                                               \
        } while (0)

    #define assert_equal_string(actual, expected)                                               \
        do {                                                                                    \
            __auto_type a = (char*)(actual);                                                    \
            __auto_type b = (char*)(expected);                                                  \
            if (strcmp(a, b) != 0) {                                                            \
                errorf(                                                                         \
                    "Assertion failed: Expected %s to be \"%s\", got \"%s\" (%s:%d in %s)\n",   \
                    #actual, b, a, __FILE__, __LINE__, __func__                                 \
                );                                                                              \
                exit(1);                                                                        \
            }                                                                                   \
        } while (0)

#else

    #define assertf(expr, ...)
    #define assert_equal(actual, expected)
    #define assert_equal_string(actual, expected)

#endif

#endif
