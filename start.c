#include "allocator.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Custom Allocator Simple Test ===\n");

    // -----------------------------
    // Step 1: my_malloc
    // -----------------------------
    printf("\nAllocating 512 bytes with my_malloc...\n");
    void *p1 = my_malloc(512);
    print_heap();

    printf("\nAllocating 1024 bytes with my_malloc...\n");
    void *p2 = my_malloc(1024);
    print_heap();

    // -----------------------------
    // Step 2: my_calloc
    // -----------------------------
    printf("\nAllocating zeroed block (200 elements of 8 bytes) with my_calloc...\n");
    void *p3 = my_calloc(200, 8);
    print_heap();

    // -----------------------------
    // Step 3: my_realloc (expand)
    // -----------------------------
    printf("\nRealloc p1 from 512 -> 1500 bytes...\n");
    p1 = my_realloc(p1, 1500);
    print_heap();

    // -----------------------------
    // Step 4: my_realloc (shrink)
    // -----------------------------
    printf("\nRealloc p2 from 1024 -> 600 bytes...\n");
    p2 = my_realloc(p2, 600);
    print_heap();

    // -----------------------------
    // Step 5: Free blocks
    // -----------------------------
    printf("\nFreeing p1...\n");
    my_free(p1);
    print_heap();

    printf("\nFreeing p2...\n");
    my_free(p2);
    print_heap();

    printf("\nFreeing p3...\n");
    my_free(p3);
    print_heap();

    printf("\n=== Test Completed ===\n");
    return 0;
}
