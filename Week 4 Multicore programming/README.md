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


---
## Homework
