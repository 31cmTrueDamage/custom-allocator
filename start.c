#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NUM_PTRS 1000       // Number of tracked pointers
#define ITERATIONS 50000    // Number of random operations
#define MAX_ALLOC_SIZE 2048 // Maximum allocation size in bytes

typedef struct {
    void *ptr;
    size_t size;
    unsigned char pattern;
} alloc_entry;

// Fill memory with a pattern
void fill_pattern(void *ptr, size_t size, unsigned char pattern) {
    memset(ptr, pattern, size);
}

// Verify memory contains the pattern
int verify_pattern(void *ptr, size_t size, unsigned char pattern) {
    unsigned char *p = (unsigned char *)ptr;
    for (size_t i = 0; i < size; i++) {
        if (p[i] != pattern) return 0;
    }
    return 1;
}

int main(void) {
    printf("=== Custom Allocator Medium Stress Test ===\n");
    srand((unsigned)time(NULL));

    alloc_entry table[NUM_PTRS] = {0};
    size_t total_allocs = 0, total_frees = 0, total_reallocs = 0, errors = 0;

    for (size_t i = 0; i < ITERATIONS; i++) {
        int action = rand() % 100;
        int index = rand() % NUM_PTRS;

        if (action < 60) { // Allocate
            if (table[index].ptr != NULL) continue;
            size_t size = (rand() % MAX_ALLOC_SIZE) + 1;
            unsigned char pattern = (unsigned char)(i & 0xFF);
            void *p = my_malloc(size);
            if (!p) {
                errors++;
                continue;
            }
            fill_pattern(p, size, pattern);
            table[index].ptr = p;
            table[index].size = size;
            table[index].pattern = pattern;
            total_allocs++;
        }
        else if (action < 80) { // Free
            if (table[index].ptr != NULL) {
                if (!verify_pattern(table[index].ptr, table[index].size, table[index].pattern)) {
                    printf("[ERROR] Memory corruption detected before free at index %d\n", index);
                    errors++;
                }
                my_free(table[index].ptr);
                table[index].ptr = NULL;
                table[index].size = 0;
                table[index].pattern = 0;
                total_frees++;
            }
        }
        else { // Realloc
            if (table[index].ptr != NULL) {
                size_t new_size = (rand() % MAX_ALLOC_SIZE) + 1;
                void *new_ptr = my_realloc(table[index].ptr, new_size);
                if (!new_ptr) {
                    errors++;
                    my_free(table[index].ptr);
                    table[index].ptr = NULL;
                    table[index].size = 0;
                    table[index].pattern = 0;
                    continue;
                }

                size_t min_size = table[index].size < new_size ? table[index].size : new_size;
                if (!verify_pattern(new_ptr, min_size, table[index].pattern)) {
                    printf("[ERROR] Memory corruption after realloc at index %d\n", index);
                    errors++;
                }

                unsigned char new_pattern = (unsigned char)(i & 0xFF);
                fill_pattern(new_ptr, new_size, new_pattern);
                table[index].ptr = new_ptr;
                table[index].size = new_size;
                table[index].pattern = new_pattern;
                total_reallocs++;
            }
        }

        if (i % 5000 == 0 && i > 0) {
            printf("[Progress] Iteration %zu / %d, errors so far: %zu\n", i, ITERATIONS, errors);
        }
    }

    for (int i = 0; i < NUM_PTRS; i++) {
        if (table[i].ptr != NULL) {
            if (!verify_pattern(table[i].ptr, table[i].size, table[i].pattern)) {
                printf("[ERROR] Memory corruption during cleanup at index %d\n", i);
                errors++;
            }
            my_free(table[i].ptr);
        }
    }

    printf("\n=== Medium Stress Test Completed ===\n");
    printf("Total allocations: %zu\n", total_allocs);
    printf("Total frees:       %zu\n", total_frees);
    printf("Total reallocs:    %zu\n", total_reallocs);
    printf("Total errors:      %zu\n", errors);
    print_heap();

    return 0;
}
