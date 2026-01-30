#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>

#include "funcs.h"

#define numFunctions 2048

double run(int n) {
    struct timespec s, e;
    clock_gettime(CLOCK_MONOTONIC, &s);

    for (int r = 0; r < numFunctions; r++) {
        for (int i = 0; i < n; ++i) {
            funcs[i]();
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &e);

    //ns -> ms
    double nanoseconds = (double)(e.tv_sec - s.tv_sec) * 1e9 + (e.tv_nsec - s.tv_nsec);
    return nanoseconds / 1e6; 
}

int main() {
    unsigned long size = 128;
    printf("------------------------------------\n");
    for (int n = 1; n <= 2048; n *= 2) {
        double kb = (n * size) / 1024.0;
        printf("%d funcs (%.1f KB): %.2f ms\n", n, kb, run(n));
    }
    printf("------------------------------------\n");

    return 0;
}