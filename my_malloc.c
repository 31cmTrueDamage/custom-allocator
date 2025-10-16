#include "allocator.h"

block_t *head = NULL;
block_t *last = NULL;

void *my_malloc(size_t size) {

    void *block_pointer;

    if (size == 0) {
        return NULL;
    }

    size = align_size(size);

    block_t *free_block = find_free_block(head ,size);

    if (!free_block) {
        block_t *new_block = request_space(last, size);
        if (head == NULL) {
            head = new_block;
            last = new_block;
        } else {
            last = new_block;
        }
        block_pointer = (void *)(new_block + 1);
    } else {
        if (free_block->size >= size + sizeof(block_t) + 1) {
            size_t calc = free_block->size - size - sizeof(block_t);
            block_t *split_block = (block_t*)((char*)free_block + sizeof(block_t) + size);
            split_block->next = free_block->next;
            split_block->free = 1;
            split_block->size = calc;
            free_block->size = size;
            split_block->prev = free_block;
            free_block->next = split_block;
            if (split_block->next) {
                split_block->next->prev = split_block;
            }
        }
        free_block->free = 0;
        block_pointer = (void *)(free_block + 1);
    }

    return block_pointer;
}