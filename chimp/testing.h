#ifndef LIBCHIMP_TESTING_H
#define LIBCHIMP_TESTING_H

#ifndef NDEBUG

    #include <stdio.h>

    #undef assert
    #undef assertf
    #undef assert_equal
    #undef assert_equal_string

    #define _ASSERTF_FORMAT "%s:%d:%s \033[31mFAIL\033[0m Assertion failed: %s\n"
    #define _ASSERT_EQUAL_FORMAT "%s:%d:%s \033[31mFAIL\033[0m Expected %s to be %lld, got %lld\n"
    #define _ASSERT_EQUAL_STRING_FORMAT "%s:%d:%s \033[31mFAIL\033[0m Expected %s to be \"%s\", got \"%s\"\n"

    #define assert(expr) assertf(expr, "\r")

    #define assertf(expr, ...)                      \
        do if (!(expr)) {                           \
            fprintf(                                \
                stderr,                             \
                _ASSERTF_FORMAT,                    \
                __FILE__, __LINE__, __func__, #expr \
            );                                      \
            fprintf(stderr, __VA_ARGS__);           \
            return 1;                               \
        } while (0)

    #define assert_equal(actual, expected)                      \
        do {                                                    \
            int64_t a = (int64_t)(actual);                      \
            int64_t b = (int64_t)(expected);                    \
            if (a != b) {                                       \
                fprintf(                                        \
                    stderr,                                     \
                    _ASSERT_EQUAL_FORMAT,                       \
                    __FILE__, __LINE__, __func__, #actual, b, a \
                );                                              \
                return 1;                                       \
            }                                                   \
        } while (0)

    #define assert_equal_string(actual, expected)               \
        do {                                                    \
            char* a = (char*)(actual);                          \
            char* b = (char*)(expected);                        \
            if (strcmp(a, b) != 0) {                            \
                fprintf(                                        \
                    stderr,                                     \
                    _ASSERT_EQUAL_STRING_FORMAT,                \
                    __FILE__, __LINE__, __func__, #actual, b, a \
                );                                              \
                return 1;                                       \
            }                                                   \
        } while (0)

#else

    #define assertf(expr, ...)
    #define assert_equal(actual, expected)
    #define assert_equal_string(actual, expected)

#endif

#endif
