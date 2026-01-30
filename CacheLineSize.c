#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>

#include "funcs.h"

#define numFunctions 2048

long run(int stride) {
    struct timespec s, e;
    clock_gettime(CLOCK_MONOTONIC, &s);

    int reps = stride; 

    for (int r = 0; r < reps; r++) {
        for (int i = 0; i < numFunctions; i += stride) {
            funcs[i]();
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &e);

    return (e.tv_sec - s.tv_sec) * 1e9 + (e.tv_nsec - s.tv_nsec);
    
}

int main() {
    unsigned long funcSize = (char*)f1 - (char*)f0;
    printf("spacing = %ld\n", (char*)f1 - (char*)f0);
    printf("------------------------------------\n");

    for (int stride = 1; stride <= 2048; stride *= 2) {
        double spacing = stride * funcSize;
        double div64 = spacing / 64.0;
        printf("stride %d (%.0f B): %ld ns | Divided by 64 = %.2f\n",
               stride, spacing, run(stride), div64);
    }

    printf("------------------------------------\n");
    return 0;
}