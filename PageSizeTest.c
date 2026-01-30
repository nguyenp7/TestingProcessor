#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define Iterations 10000000
#define MaxArraySize 33554432 //32MB

long double test_page_size(int stride) {
    struct timespec s, e;
    uint32_t *arr = malloc(MaxArraySize);
    int num_elements = MaxArraySize / stride;

    for (int i = 0; i < num_elements - 1; i++) {
        arr[(i * stride) / 4] = (i + 1) * stride;
    }
    
    arr[((num_elements - 1) * stride) / 4] = 0; 

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
    printf("--- Probing Page Size ---\n");
    for (int stride = 1024; stride <= 32768; stride *= 2) {
        long double ns = test_page_size(stride);
        printf("Stride %d bytes: %.5Lf ns/access\n", stride, ns);
    }
    printf("-------------------------\n");
    return 0;
}