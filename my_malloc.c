#include "allocator.h"
#include <stdio.h>

block_t *head = NULL;
block_t *last = NULL;

void *my_malloc(size_t size) {
    if (size == 0) return NULL;
    size = align_size(size);
    block_t *free_block = find_free_block(head, size);
    void *block_pointer;
    if (!free_block) {
        block_t *new_block = request_space(last, size);
        if (!head) head = new_block;
        last = new_block;
        block_pointer = (void *)(new_block + 1);
        printf("[MALLOC] new block=%p, size=%zu\n", block_pointer, size);
    } else {
        if (free_block->size >= size + sizeof(block_t) + 1) {
            size_t calc = free_block->size - size - sizeof(block_t);
            block_t *split_block = (block_t *)((char *)free_block + sizeof(block_t) + size);
            split_block->next = free_block->next;
            split_block->free = 1;
            split_block->size = calc;
            free_block->size = size;
            split_block->prev = free_block;
            free_block->next = split_block;
            if (split_block->next) split_block->next->prev = split_block;
            printf("[MALLOC] split block=%p, size=%zu\n", split_block, calc);
        }
        free_block->free = 0;
        block_pointer = (void *)(free_block + 1);
        printf("[MALLOC] reuse block=%p, size=%zu\n", block_pointer, free_block->size);
    }
    return block_pointer;
}
