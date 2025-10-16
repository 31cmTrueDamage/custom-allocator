#include "allocator.h"
#include <stdio.h>

void print_heap() {
    block_t *curr = head;
    printf("Heap Blocks:\n");
    while (curr) {
        printf("Block %p | size: %zu | free: %d | prev: %p | next: %p\n",
               curr,
               curr->size,
               curr->free,
               curr->prev,
               curr->next);
        curr = curr->next;
    }
    printf("End of heap\n\n");
}