#include "arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALIGN_32 (4)
#define ALIGN_64 (8)
#define DEFAULT_ALIGN ALIGN_64

size_t align_up(size_t offset, size_t alignment) {
    size_t mask = alignment - 1;
    return (offset + mask) & ~mask;
}

void arena_make(Arena *arena, size_t size) {
    arena->base = malloc(size);
    assert(arena->base);

    memset(arena->base, 0, size);

    arena->offset = arena->base;
    arena->size = size;
}

void arena_free(Arena *arena) {
    free(arena->base);
}

void arena_clear(Arena *arena) {
    memset(arena->base, 0, arena->size);
    arena->offset = arena->base;
}

void *arena_alloc(Arena *arena, size_t size) {
    size_t offset = arena->offset - arena->base;
    size_t align_offset = align_up(offset, DEFAULT_ALIGN);
    if (align_offset + size > arena->size) {
        printf("cannot do arena_alloc, size exceeds arena limit\n");
        return NULL;
    }

    void *start = arena->offset;
    arena->offset += size;

    return start;
}
