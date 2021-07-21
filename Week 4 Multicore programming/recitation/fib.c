#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int64_t fib(int64_t n) {
    if (n < 2) {
        return n;
    }
    else {
        int64_t x = fib(n - 1);
        int64_t y = fib(n - 2);
        return (x + y);
    }
}

int main(int argc, char* argv[]) {
    int64_t n = atoi(argv[1]);
    int64_t result = fib(n);
    printf("Fibonacci of %" PRId64 " is %" PRId64 ".\n", n, result);
}
