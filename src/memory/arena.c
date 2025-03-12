#include "arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Virtual memory allocation for Windows
// https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc

// Virtual memory allocation for Linux
// https://www.man7.org/linux/man-pages/man2/mmap.2.html

void arena_make(Arena* arena, size_t capacity) {
    arena->base = malloc(capacity);
    assert(arena->base);

    memset(arena->base, 0, capacity);

    arena->offset = arena->base;
    arena->capacity = capacity;
}

void arena_free(Arena* arena) {
    free(arena->base);
}

void arena_clear(Arena* arena) {
    memset(arena->base, 0, arena->capacity);
    arena->offset = arena->base;
}

void* arena_alloc(Arena *arena, size_t size) {
    size_t offset = arena->offset - arena->base;

    printf("size: %lu\n", size);
    printf("offset: %lu\n", offset);

    if (offset + size > arena->capacity) {
        printf("cannot do arena_alloc, size exceeds arena limit\n");
        return NULL;
    }

    void* start = arena->offset;
    arena->offset += size;

    return start;
}
