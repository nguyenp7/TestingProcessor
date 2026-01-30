#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define ITERATIONS 10000000
#define WORKING_SET (32 * 1024) // Stay within L1 (32KB) to isolate Line Size

long double test_line_size(int stride) {
    struct timespec s, e;
    // We allocate a small buffer that fits in L1
    uint8_t *arr = malloc(WORKING_SET);
    for (int i = 0; i < WORKING_SET; i++) arr[i] = (uint8_t)(i % 256);

    // We use a simple loop, but 'volatile' and 'sum' to prevent optimization
    volatile uint8_t sink;
    size_t mask = WORKING_SET - 1;

    clock_gettime(CLOCK_MONOTONIC, &s);
    for (int i = 0; i < ITERATIONS; i++) {
        // Accessing with a stride. 
        // If stride < CacheLine, several 'i' will land in the same line.
        sink = arr[(i * stride) & mask];
    }
    clock_gettime(CLOCK_MONOTONIC, &e);

    long double total_ns = (long double)(e.tv_sec - s.tv_sec) * 1e9L + (e.tv_nsec - s.tv_nsec);
    free(arr);
    return total_ns / ITERATIONS;
}

int main() {
    printf("Stride (Bytes) | Latency (ns/access)\n");
    printf("------------------------------------\n");
    
    // We test every power of 2 from 4 bytes up to 1024
    for (int stride = 4; stride <= 256; stride *= 2) {
        long double ns = test_line_size(stride);
        printf("%14d | %.5Lf\n", stride, ns);
    }
    printf("------------------------------------\n");
    return 0;
}