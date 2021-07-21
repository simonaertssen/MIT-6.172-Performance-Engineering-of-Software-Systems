#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <omp.h>


int64_t fib(int64_t n) {
    if (n < 2) return n;
    int64_t x, y;
#pragma omp task shared(x)
    x = fib(n - 1);
#pragma omp task shared(y)
    y = fib(n - 2);
#pragma omp taskwait
    return (x + y);
}

int main(int argc, char* argv[]) {
    int64_t n = atoi(argv[1]);

    omp_set_num_threads(3);
    int nthreads = omp_get_num_threads();
    printf("N = %d", nthreads);

#pragma omp parallel
    {
#pragma omp single
        {
            int64_t result = fib(n);
            printf("Fibonacci of %" PRId64 " is %" PRId64 ".\n", n, result);
        }
    }
}
