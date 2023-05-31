#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct Arena Arena;

/**
 * Creates a memory arena.
*/
Arena* arena_init(void);

/**
 * Allocs n_bytes from the arena.
*/
void* arena_alloc(Arena *arena, size_t n_bytes);

/**
 * Allocs n_bytes from the arena, aligned to alignment bytes.
*/
void* arena_aligned_alloc(Arena *arena, size_t n_bytes, size_t alignment);

/**
 * Clears the arena, retaining the reserved memory.
 */
void arena_clear(Arena *arena);

/**
 * Frees all the memory in the arena.
*/
void arena_free(Arena *arena);

#endif // ARENA_H
