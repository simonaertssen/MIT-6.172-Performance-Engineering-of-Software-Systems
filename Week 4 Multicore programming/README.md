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

---
## Homework
