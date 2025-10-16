#include "allocator.h"

#define ALIGNMENT 8

block_t *request_space(block_t *last_block, size_t size) {
    if (size == 0) {
        return NULL;
    }

    void *request = sbrk(sizeof(block_t) + size);

    if (request == (void*) -1) {
        return NULL;
    }

    block_t *newblock = (block_t *)request;

    newblock->size = size;
    newblock->free = 0;
    newblock->next=NULL;
    newblock->prev=last_block;

    if (last_block) {
        last_block->next=newblock;
    }

    return newblock;
}

size_t align_size(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

block_t *find_free_block(block_t *start, size_t size) {
    block_t *curr = start;
    block_t *bestfit = NULL;
    size_t shortest_diff = __SIZE_MAX__;
    while (curr) {
        if (curr->size == size) {
            return curr;
        }
        if (curr->free == 1 && curr->size >= size) {
            size_t calc = curr->size - size;
            if (calc < shortest_diff) {
                shortest_diff = calc;
                bestfit = curr;
            }
        }
        curr=curr->next;
    }
    return bestfit;
}

void merge_blocks(block_t *freed_block) {

    if (!freed_block) {
        return;
    }

    if (freed_block->prev && freed_block->prev->free == 1) {
        freed_block->prev->size += freed_block->size + sizeof(block_t);
        freed_block->prev->next = freed_block->next;
        if (freed_block->next) {
            freed_block->next->prev = freed_block->prev;
        }
        freed_block = freed_block->prev;
    }
    if (freed_block->next && freed_block->next->free == 1) {
        freed_block->size += freed_block->next->size + sizeof(block_t);
        freed_block->next = freed_block->next->next;
        if (freed_block->next) {
            freed_block->next->prev = freed_block;
        }
    }
}