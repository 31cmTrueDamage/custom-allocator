#include "allocator.h"

void *my_calloc(size_t nitems, size_t size) {

    if (nitems <= 0 || size <= 0) {
        return NULL;
    }

    size_t amount = size * nitems;
    if (size != 0 && amount / size != nitems) return NULL;

    void *ptr = my_malloc(amount);
    memset(ptr, 0, amount);

    return ptr;
}   