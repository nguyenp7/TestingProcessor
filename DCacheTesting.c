#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MaxArraySize 65536
#define Iterations 10000000
long double run(int size) {
    struct timespec s, e;
    int *arr = malloc(size*4);
    for(int i = 0; i < size; ++i){
        arr[i] = i;
    }

    //Shuffle The Array
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }


    volatile int p = 0;
    clock_gettime(CLOCK_MONOTONIC, &s);

    //bounce around array
    for (int i = 0; i < Iterations; i++) {
        p = arr[p]; 
    }

    clock_gettime(CLOCK_MONOTONIC, &e);
    
    long double total_ns = (long double)(e.tv_sec - s.tv_sec) * 1e9L + (e.tv_nsec - s.tv_nsec);
    free(arr);
    return total_ns / Iterations;
}

int main() {
    printf("------------------------------------\n");
    
    for (int size = 2048; size <= MaxArraySize; size *= 2) {
        double kb = (sizeof(int) * size) / 1024.0;
        long double ns_per_access = run(size);
        printf("%d elements (%.1f KB) %.5Lf ns per access\n", size, kb, ns_per_access);
    }
    printf("------------------------------------\n");
}
