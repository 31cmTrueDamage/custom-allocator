#include "allocator.h"
#include <stdio.h>
#include <string.h>

void *my_realloc(void *ptr, size_t size) {
    if (!ptr) {
        if (size == 0) return NULL;
        void *p = my_malloc(size);
        printf("[REALLOC] NULL -> new ptr=%p, size=%zu\n", p, size);
        return p;
    }

    if (size == 0) {
        my_free(ptr);
        printf("[REALLOC] ptr=%p -> size 0, freed\n", ptr);
        return NULL;
    }

    size = align_size(size);
    block_t *block_to_resize = (block_t *)((char *)ptr - sizeof(block_t));
    size_t old_size = block_to_resize->size;

    if (size == old_size) {
        return ptr;
    }

    if (size < old_size) {
        size_t leftover = old_size - size;
        if (leftover >= sizeof(block_t) + 16) {
            block_t *split_block = (block_t *)((char *)block_to_resize + sizeof(block_t) + size);
            split_block->size = leftover - sizeof(block_t);
            split_block->free = 1;
            split_block->next = block_to_resize->next;
            split_block->prev = block_to_resize;
            block_to_resize->next = split_block;
            block_to_resize->size = size;
            if (split_block->next) split_block->next->prev = split_block;
            printf("[REALLOC] shrink with split: ptr=%p, new_size=%zu, split=%p, split_size=%zu\n",
                   ptr, size, split_block, split_block->size);
        } else {
            block_to_resize->size = size;
            printf("[REALLOC] shrink without split: ptr=%p, new_size=%zu\n", ptr, size);
        }
        return ptr;
    }

    if (size > old_size) {
        if (block_to_resize->next && block_to_resize->next->free &&
            (old_size + sizeof(block_t) + block_to_resize->next->size) >= size) {
            block_to_resize->size += sizeof(block_t) + block_to_resize->next->size;
            block_to_resize->next = block_to_resize->next->next;
            if (block_to_resize->next) block_to_resize->next->prev = block_to_resize;
            printf("[REALLOC] expanded in place: ptr=%p, new_size=%zu\n", ptr, size);
            return ptr;
        } else {
            void *new_ptr = my_malloc(size);
            if (!new_ptr) {
                printf("[REALLOC] failed to allocate new block for ptr=%p, size=%zu\n", ptr, size);
                return NULL;
            }
            memcpy(new_ptr, (char *)block_to_resize + sizeof(block_t), old_size);
            my_free(ptr);
            printf("[REALLOC] moved ptr=%p -> new_ptr=%p, new_size=%zu\n", ptr, new_ptr, size);
            return new_ptr;
        }
    }

    return ptr;
}
