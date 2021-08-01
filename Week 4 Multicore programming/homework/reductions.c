/*
Here we will investigate the performance of reductions using omp.
We will take different reductions on consecutive numbers.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "./fasttime.h"

#define NUM_THREADS 10

void reduce_ADD(uint64_t);
void reduce_MUL(uint64_t);
void reduce_AND(uint64_t);
void reduce_OR_(uint64_t);

int main(int argc, char* argv[]) {
    uint64_t N = 1000000000000000;
    reduce_ADD(N);
    return 0;
}

void reduce_ADD(uint64_t N) {
    size_t i;
    uint64_t result = 0;
    fasttime_t start_time, endin_time;

    // Parallel execution 
    start_time = gettime();
#pragma omp parallel for reduction(+ : result) num_threads(1)
    for (i = 0; i < N; i++) {
        result += i;
    }
    endin_time = gettime();
    printf("%s: parall %f secs has value %llu\n", __func__, tdiff(start_time, endin_time), result);

    // Serial execution 
    start_time = gettime();
    for (i = 0; i < N; i++) {
        result -= i;
    }
    endin_time = gettime();
    printf("%s: serial %f secs has value %llu\n", __func__, tdiff(start_time, endin_time), result);

    // Check results
    if (result != 0) printf("Error in function %s, result is not correct.\n", __func__);

    return;
}
