#ifndef LIBCHIMP_SLICE_ARENA_H
#define LIBCHIMP_SLICE_ARENA_H

#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct Slice_Arena Slice_Arena;
struct Slice_Arena {
    uint8_t* const buffer;
    size_t const size;
    uint64_t offset;
};

typedef struct Arena_Slice Arena_Slice;
struct Arena_Slice {
    void* const pointer;
    size_t const size;
};

// Create a slice arena with offset set to 0.
Slice_Arena slice_arena_create(
    uint8_t* const buffer,
    size_t const size
) {
    assert(buffer != NULL);
    return (Slice_Arena) {
        .buffer = buffer,
        .size = size,
        .offset = 0,
    };
}

// Align the pointer forward if needed.
// Reference: https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
uintptr_t slice_arena_align_forward(uintptr_t const pointer) {
    uintptr_t const alignment = (uintptr_t)(2 * sizeof(void*));
    uintptr_t const modulo = pointer & (alignment - 1);

	if (modulo != 0) {
        return pointer + alignment - modulo;
	}

	return pointer;
}

// Allocate memory in the arena.
// If there's not enough memory, the pointer will be NULL.
Arena_Slice slice_arena_alloc(
    Slice_Arena* const arena,
    size_t const size
) {
    assert(arena != NULL);
    assert(arena->buffer != NULL);
    assert(arena->offset < arena->size);

    void* pointer = NULL;

    uintptr_t const offset_pointer = (uintptr_t)arena->buffer + (uintptr_t)arena->offset;
    uintptr_t const aligned_offset = slice_arena_align_forward(offset_pointer) - (uintptr_t)arena->buffer;

    if (aligned_offset + size <= arena->size) {
        pointer = (void*)&arena->buffer[aligned_offset];
        arena->offset += size;
        memset(pointer, 0, size);
    }

    return (Arena_Slice) {
        .pointer = pointer,
        .size = size,
    };
}

// Zero out the arena buffer entirely.
// Set the offset to 0.
void slice_arena_clear(Slice_Arena* const arena) {
    assert(arena != NULL);
    assert(arena->buffer != NULL);
    assert(arena->offset < arena->size);

    memset(arena->buffer, 0, arena->size);
    arena->offset = 0;
}

#endif
