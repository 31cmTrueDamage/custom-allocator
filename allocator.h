#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

typedef struct block {
    int free;
    size_t size;
    struct block *prev;
    struct block *next;
} block_t;

extern block_t *head;
extern block_t *last;

size_t align_size(size_t size);
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nitems, size_t size);
void my_free(void *ptr);
void merge_blocks(block_t *freed_block);
block_t *request_space(block_t *lastBlock, size_t size);
block_t *find_free_block(block_t *start, size_t size);
void print_heap(void);

#endif
