#include "allocator.h"

void my_free(void *ptr) {
    if (!ptr) {
        return;
    }
    block_t *block_to_free = (block_t *)ptr - 1;
    block_to_free->free=1;
    merge_blocks(block_to_free);
}