/**
 * Copyright (c) 2014 MIT License by 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./fasttime.h"

// N is small enough so that 3 arrays of size N fit into the AWS machine
// level 1 caches (which are 32 KB each, as seen by running `lscpu`)
#define N          1024

// Run for multiple experiments to reduce measurement error on gettime().
#define I          100000

// Which operations are vectorizable?
// Guarding it with #ifndef allows passing -D"__OP__=$value" on the
// command line
#ifndef __OP__
#define __OP__     +
#endif
#ifndef __TYPE__
#define __TYPE__   uint32_t
#endif

// Define a way of automatically converting __OP__ and __TYPE__ into string literals
#define stringify(V) _stringify(V)
#define _stringify(V) #V

int main(int argc, char *argv[]) {
    __TYPE__ A[N];
    __TYPE__ B[N];
    __TYPE__ C[N];
    __TYPE__ total = 0;

    int i, j;
    unsigned int seed = 0;

    // Touch each element in each array before we start the timed part
    // of execution.  This operation brings all arrays into the level 1
    // cache and gives us a 'cleaner' view of speedup from vectorization.
    for (j = 0; j < N; j++) {
        A[j] = 0;  // 0 was chosen arbitrarily
        B[j] = 0;
        C[j] = 0;
    }

    fasttime_t time1 = gettime();

    for (i = 0; i < I; i++) {
        for (j = 0; j < N; j++) {
            C[j] = A[j] __OP__ B[j];
        }
    }

    fasttime_t time2 = gettime();

    // Forces the compiler to not prune away any loop operations
    total += C[rand_r(&seed) % N];

    double elapsedf = tdiff(time1, time2);
    // C concatenates adjacent string literals.  We take advantage of
    // this and include a print-out of __OP__ and __TYPE__
    printf("Elapsed execution time: %f sec; N: %d, I: %d,"
           " __OP__: %s, __TYPE__: %s\n",
           elapsedf, N, I, stringify(__OP__), stringify(__TYPE__));

    return total;
}
