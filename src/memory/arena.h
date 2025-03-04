#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct Arena {
    void* offset;
    void* base;
    size_t capacity; 
} Arena;

void arena_make(Arena *arena, size_t capacity);
void arena_free(Arena *arena);
void arena_clear(Arena *arena);
void* arena_alloc(Arena *arena, size_t capacity);

#endif // ARENA_H