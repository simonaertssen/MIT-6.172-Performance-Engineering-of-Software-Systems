## Recitation
Tried installing cilkplus by following the instructions here: <https://cilkplus.github.io/>
There seemed to be the following error:

    error: no member named 'ptrdiff_t' in the global namespace

Let's see if we can start by using OpenMP first.


### Checkoff Item 1
For a program running on a single core, we run `time ./fib 40` and obtain:
    
    real    0m0.530s
    user    0m0.517s
    sys     0m0.004s

After parallelisation, we obtain:

    real    1m0.638s
    user    2m37.992s
    sys     0m33.100s

This is significanlty slower! The number of spawn/task calls has a large overhead. 

### Checkoff Item 2
We can easily coarsen the program by testing if `n` is small enough and then performing the computations serially. We then obtain:

    real    0m2.247s
    user    0m8.327s
    sys     0m0.032s

This is not really a speedup, but perhaps we could finetune the coarsness.

### Checkoff Item 3
Testing the serial approach, `time ./transpose 10000` yields:

    real    0m1.705s
    user    0m1.614s
    sys     0m0.080s

Using the parallel version we simply go down to:

    real    0m1.392s
    user    0m2.689s
    sys     0m0.140s

That is a small speedup! The `private` directive declares data to have a separate copy in the memory of each thread. The `num_threads` directive sets the number of threads to be used for that parallel section. The `schedule` directive is a specification of how iterations of associated loops are divided into contiguous non-empty subsets.

---
## Homework
The way `queens.c` works is by using a `1` bit to represent a queen's position, in a 8-bit number. First, we check whether all rows have a queen, by AND-ing with 255 (`0xFF`). Then, `poss` is nonzero if any bit in `row`, `left` or `right` is NOT set yet (that is why we NOT all ORs and AND with 255). `place` contains the location where a queen is put, as we do not use all of the available bits in the 4-byte integer. We then need to try a row where the place is taken.