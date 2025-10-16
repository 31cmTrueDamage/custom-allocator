#include "allocator.h"
#include <stdio.h>
#include <string.h>

void *my_calloc(size_t nitems, size_t size) {
    if (nitems <= 0 || size <= 0) return NULL;
    size_t amount = nitems * size;
    void *ptr = my_malloc(amount);
    memset(ptr, 0, amount);
    printf("[CALLOC] ptr=%p, size=%zu\n", ptr, amount);
    return ptr;
}
