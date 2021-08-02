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
    uint64_t N = 1000000000;
    reduce_ADD(N);
    return 0;
}

void reduce_ADD(uint64_t N) {
    size_t i;
    uint64_t parall_result = 0;
    uint64_t serial_result = 0;
    fasttime_t start_time, endin_time;

    // Test addition
    start_time = gettime();
#pragma omp parallel for reduction(+ : parall_result) num_threads(NUM_THREADS)
    for (i = 0; i < N; i++) {
        parall_result += i;
    }
    endin_time = gettime();
    printf("%s: parall ADD in %f secs\n", __func__, tdiff(start_time, endin_time));

    start_time = gettime();
    for (i = 0; i < N; i++) {
        serial_result += i;
    }
    endin_time = gettime();
    printf("%s: serial ADD in %f secs\n", __func__, tdiff(start_time, endin_time));

    // Check results
    if (parall_result != serial_result) printf("Error in function %s, result is not correct.\n", __func__);

    // Test subtraction
    start_time = gettime();
#pragma omp parallel for reduction(- : parall_result) num_threads(NUM_THREADS)
    for (i = 0; i < N; i++) {
        parall_result -= i;
    }
    endin_time = gettime();
    printf("%s: parall SUB in %f secs\n", __func__, tdiff(start_time, endin_time));

    start_time = gettime();
    for (i = 0; i < N; i++) {
        serial_result += i;
    }
    endin_time = gettime();
    printf("%s: serial SUB in %f secs\n", __func__, tdiff(start_time, endin_time));

    // Check results
    if (parall_result != 0 | serial_result != 0 | parall_result != serial_result)
        printf("Error in function %s, result is not correct.\n", __func__);

    return;
}
