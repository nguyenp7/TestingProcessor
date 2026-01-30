#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define Iterations 10000000
#define NUM_ENTRIES 16384 

long double run(int stride) {
    struct timespec s, e;
    uint64_t total_mem = NUM_ENTRIES * stride;
    uint32_t *arr = malloc(total_mem);

    for (int i = 0; i < NUM_ENTRIES - 1; i++) {
        arr[(i * stride) / 4] = (i + 1) * stride;
    }
    arr[((NUM_ENTRIES - 1) * stride) / 4] = 0; 

    volatile uint32_t p = 0;
    clock_gettime(CLOCK_MONOTONIC, &s);
    for (int i = 0; i < Iterations; i++) {
        p = arr[p / 4]; 
    }
    clock_gettime(CLOCK_MONOTONIC, &e);
    
    free(arr);
    long double total_ns = (long double)(e.tv_sec - s.tv_sec) * 1e9L + (e.tv_nsec - s.tv_nsec);
    return total_ns / Iterations;
}

int main() {
    printf("-------------------------\n");
    for (int stride = 1024; stride <= 32768; stride *= 2) {
        long double ns = run(stride);
        printf("Stride %d bytes: %.5Lf ns/access\n", stride, ns);
    }
    printf("-------------------------\n");
    return 0;
}