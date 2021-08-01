/*
Here we will investigate the performance of reductions using omp.
We will take different reductions on consecutive numbers.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "./fasttime.h"

#define N 100000000
#define NUM_THREADS 1

void reduce_ADD(void);
void reduce_MUL(void);
void reduce_AND(void);
void reduce_OR_(void);

int main(int argc, char* argv[]) {

    reduce_ADD();

    return 0;
}

void reduce_ADD(void) {
    size_t i;
    uint64_t result = 0;
    fasttime_t start_time, endin_time;

    // Parallel execution 
    start_time = gettime();
#pragma omp parallel for reduction(+ : result)
    for (i = 0; i < N; i++) {
        result += i;
    }
    endin_time = gettime();
    printf("%s: parall %f s\n", __func__, tdiff(start_time, endin_time));

    // Serial execution 
    start_time = gettime();
    for (i = 0; i < N; i++) {
        result -= i;
    }
    endin_time = gettime();
    printf("%s: serial %f s\n", __func__, tdiff(start_time, endin_time));


    // Check results
    if (result != 0) printf("Error in function %s, result is not correct.\n", __func__);

    return;
}
