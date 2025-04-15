#ifndef LIBCHIMP_ARENA_H
#define LIBCHIMP_ARENA_H

#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct Arena Arena;
struct Arena {
    uint8_t* const buffer;
    size_t const size;
    uint64_t offset;
};

// Create an arena with offset set to 0.
__attribute__((warn_unused_result))
Arena arena_create(
    uint8_t* const buffer,
    size_t const size
) {
    assert(buffer != NULL);
    assert(size > 0);
    return (Arena) {
        .buffer = buffer,
        .size = size,
        .offset = 0,
    };
}

// Align the pointer forward if needed.
// Reference: https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
__attribute__((warn_unused_result))
uintptr_t arena_align_forward(uintptr_t const pointer) {
    uintptr_t const alignment = (uintptr_t)(2 * sizeof(void*));
    uintptr_t const modulo = pointer & (alignment - 1);

	if (modulo != 0) {
        return pointer + alignment - modulo;
	}

	return pointer;
}

// Allocate memory in the arena.
// If there's not enough memory, the pointer will be NULL.
__attribute__((warn_unused_result))
void* arena_alloc(
    Arena* const arena,
    size_t const size
) {
    assert(arena != NULL);
    assert(arena->buffer != NULL);
    assert(arena->offset < arena->size);

    void* pointer = NULL;

    uintptr_t const offset_pointer = (uintptr_t)arena->buffer + (uintptr_t)arena->offset;
    uintptr_t const aligned_offset = arena_align_forward(offset_pointer) - (uintptr_t)arena->buffer;

    if (aligned_offset + size <= arena->size) {
        pointer = (void*)&arena->buffer[aligned_offset];
        arena->offset += size;
        memset(pointer, 0, size);
    }

    return pointer;
}

// Zero out the arena buffer entirely.
// Set the offset to 0.
void arena_clear(Arena* const arena) {
    assert(arena != NULL);
    assert(arena->buffer != NULL);
    assert(arena->offset < arena->size);

    memset(arena->buffer, 0, arena->size);
    arena->offset = 0;
}

#endif
