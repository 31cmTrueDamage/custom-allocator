#include "allocator.h"
#include <string.h>

void *my_realloc(void *ptr, size_t size) {

    if (size == 0) {
        return NULL;
    }

    size = align_size(size);

    block_t *block_to_resize = (block_t *)ptr - 1;

    size_t old_size = block_to_resize->size;

    if (size < old_size) {
        size_t leftover = old_size - size;
        if (leftover >= sizeof(block_t) + 16) {
            size_t calc = leftover - sizeof(block_t);
            block_t *split_block = (block_t*)((char*)block_to_resize + sizeof(block_t) + size);
            split_block->next = block_to_resize->next;
            split_block->free = 1;
            split_block->size = calc;
            block_to_resize->size = size;
            split_block->prev = block_to_resize;
            block_to_resize->next = split_block;
            if (split_block->next) {
                split_block->next->prev = split_block;
            }
            merge_blocks(split_block);
        } else if (block_to_resize->next && block_to_resize->next->free) {
            block_to_resize->next->size += leftover;
        }
        return (void *)(block_to_resize + 1);
    }

    if (size > old_size) {
        if (block_to_resize->next && block_to_resize->next->free &&
            (old_size + sizeof(block_t) + block_to_resize->next->size) >= size) {
            merge_blocks(block_to_resize);
            block_to_resize->size = size;
            return (void *)(block_to_resize + 1);
        } else {
            void* new_ptr = my_malloc(size);
            if (new_ptr) {
                memcpy(new_ptr, block_to_resize + 1, old_size);
                my_free(block_to_resize + 1);
                return new_ptr;
            } else {
                return NULL;
            }
        }
    }

    return (void *)(block_to_resize + 1);
}
