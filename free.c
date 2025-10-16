#include "allocator.h"
#include <stdio.h>

void my_free(void *ptr) {
    if (!ptr) return;
    block_t *block_to_free = (block_t *)((char *)ptr - sizeof(block_t));
    printf("[FREE] ptr=%p, size=%zu\n", ptr, block_to_free->size);
    block_to_free->free = 1;
    merge_blocks(block_to_free);
}
