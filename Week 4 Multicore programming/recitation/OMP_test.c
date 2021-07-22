// OpenMP program to print Hello World using C language
// Run with gcc -Xpreprocessor -fopenmp -o OMP_test OMP_test.c -lomp

#include "omp.h"
#include <stdio.h>

int main() {
    omp_set_dynamic(1);
    omp_set_num_threads(4);
    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();

        int nthreads = omp_get_num_threads();

        int max_threads = omp_get_max_threads();

        int num_proc = omp_get_num_procs();

        printf("I'm thread %d out of %d threads. There are max %d threads on %d processors.\n",
            thread_id, nthreads, max_threads, num_proc);
    }
    return 0;
}
