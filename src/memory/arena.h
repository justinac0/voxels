#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct Arena {
    void *offset;
    void *base;
    size_t size; 
} Arena;

void arena_make(Arena *arena, size_t size);
void arena_free(Arena *arena);
void arena_clear(Arena *arena);
void *arena_alloc(Arena *arena, size_t size);

typedef struct ArenaTemp {
    Arena *arena;
    void *start;
    void *prev;
} ArenaTemp;

#endif // ARENA_H