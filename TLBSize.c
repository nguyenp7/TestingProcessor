#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define Iterations 10000000
#define PAGE_SIZE 4096 

long double run(int num_pages) {
    struct timespec s, e;
    uint64_t total_mem = num_pages * PAGE_SIZE;
    uint32_t *arr = malloc(total_mem);

    for (int i = 0; i < num_pages - 1; i++) {
        arr[(i * PAGE_SIZE) / 4] = (i + 1) * PAGE_SIZE;
    }
    arr[((num_pages - 1) * PAGE_SIZE) / 4] = 0;

    volatile uint32_t p = 0;
    clock_gettime(CLOCK_MONOTONIC, &s);

    for (int i = 0; i < Iterations; i++) {
        p = arr[p / 4]; 
    }

    clock_gettime(CLOCK_MONOTONIC, &e);
    
    long double total_ns = (long double)(e.tv_sec - s.tv_sec) * 1e9L + (e.tv_nsec - s.tv_nsec);
    free(arr);
    return total_ns / Iterations;
}

int main() {
    printf("--------------------------------------------------\n");
    for (int n = 32; n <= 1024; n *= 2) {
        long double ns = run(n);
        printf("Pages Touched %d (%d KB) %.5Lf ns\n", n, (n * 4), ns);
    }
    printf("--------------------------------------------------\n");
    return 0;
}