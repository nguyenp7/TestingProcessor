#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define Iterations 10000000
#define Stride 1024 
#define MaxWays 16

long double run(int ways) {
    struct timespec s, e;
    int size = ways * Stride; 
    int *arr = malloc(size*4);
    for (int i = 0; i < size; i++) arr[i] = i;

    //make a trail of 'Stride' distance in the array. 
    for (int i = 0; i < ways; i++) {
        int current = i * Stride;
        int next = (i + 1) * Stride;

        if (i == ways - 1) {
            arr[current] = 0; 
        } else {
            arr[current] = next;
        }
    }
    
    volatile int p = 0;
    clock_gettime(CLOCK_MONOTONIC, &s);
    for (int i = 0; i < Iterations; i++) {
        p = arr[p];
    }
    clock_gettime(CLOCK_MONOTONIC, &e);

    free(arr);
    return ((long double)(e.tv_sec - s.tv_sec) * 1e9L + (e.tv_nsec - s.tv_nsec));
}

int main() {
    printf("------------------------------------\n");
    
    for (int ways = 4; ways <= MaxWays; ways++) {
        long double ns = run(ways);
        long double ns_per_access = ns/Iterations;
        printf("%d ways: %.5Lf ns per access\n", ways, ns_per_access);
    }
    printf("------------------------------------\n");
}
