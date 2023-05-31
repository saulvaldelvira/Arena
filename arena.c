#include "arena.h"
#include <stdlib.h>
#include <stdint.h>

typedef int8_t byte;

#define max_aligned_size sizeof(max_align_t)

#define PAGE_SIZE 4096
#define GROW_FACTOR 2

struct Arena {
        byte *chunk;
        size_t size;
        size_t offset;
        struct Arena *next;
};

static Arena* _arena_init(size_t page_size){
        Arena *arena = malloc(sizeof(*arena));
        if (!arena)
                return NULL;
        arena->chunk = malloc(page_size);
        if (!arena->chunk){
                free(arena);
                return NULL;
        }
        arena->size = page_size;
        arena->offset = 0;
        arena->next = NULL;
        return arena;
}

Arena* arena_init(void){
        return _arena_init(PAGE_SIZE);
}

void* arena_aligned_alloc(Arena *arena, size_t n_bytes, size_t alignment){
	if (!arena || n_bytes == 0 || alignment == 0)
		return NULL;
        Arena *tail = NULL;
        while (arena){
                tail = arena;
                uintptr_t ptr = (uintptr_t)(arena->chunk + arena->offset);
                int offset = alignment - (ptr % alignment);
                if (arena->size - arena->offset >= offset + n_bytes){
                        arena->offset += offset + n_bytes;
                        return (void*)(ptr + offset);
                }
                arena = arena->next;
        }
        // If no page has enough space, create a new one
        size_t next_size = tail->size * GROW_FACTOR + alignment;
        if (next_size < n_bytes)
                next_size = n_bytes;
        tail->next = _arena_init(next_size);
        if (!tail->next)
                return NULL;
        tail = tail->next;
        uintptr_t ptr = (uintptr_t)tail->chunk; // tail->offset is 0
        int offset = alignment - (ptr % alignment);
        tail->offset += offset + n_bytes;
        return (void*)(ptr + offset);
}

void* arena_alloc(Arena *arena, size_t n_bytes){
        return arena_aligned_alloc(arena, n_bytes, max_aligned_size);
}

void arena_clear(Arena *arena){
	while (arena){
		arena->offset = 0;
		arena = arena->next;
	}
}

void arena_free(Arena *arena){
        while (arena){
                Arena *delete = arena;
                arena = arena->next;
                free(delete->chunk);
                free(delete);
        }
}
