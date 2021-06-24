# Project 1 Writeup:

## Understanding the test suite
### Testing options
With the `-s`, `-m` and `-l` options, increasing time limits are given for the everybit program to test performance. Testing with `-t` is only a test for correctness.

### Allocating arrays
At all times only one array pointer is used, globally declared under the name `test_bitarray`. The memory buffer is freed at every test and filled with random bits.

The bitarrays are allocated in steps using a Fibonacci sequence:
- a bitarray with a size of 5 bytes is tested, with a subarray of 3 bits, shifted by 2 places, using an offset of 1.
- a bitarray with a size of 8 bytes is tested, with a subarray of 5 bits, shifted by 3 places, using an offset of 2.

## Program functionality
How does `everybit` actually work? Let's first look at the call trace of the tests:

`main > timed_rotation > testutil_rotate > bitarray_rotate > bitarray_rotate_left > bitarray_rotate_left_one`

So a lot of work is being done by the individual rotations! Perhaps we can exchange values in the array instead of rotating every bit one by one.

- `bitarray_rotate`: assert whether the bounds for the subarray are actually contained within the bitarray. Remove full rotations by only shifting the number of bit shifts modulo the length of the subarray.

- `bitarray_rotate_left`: for every bit shift that we need to perform, shift the whole subarray one bit to the left. Notice though, that this shifts to the right, due to the binary notation!

- `bitarray_rotate_left_one`: the first bit is extracted, for repurposment later. Loop from the offset through the length of the subarray and replace each bit with the next. Then put the first bit in the array again.

- `bitarray_get`: first get the byte at the desired index (per pack of 8) and then mask with a number to get the bit value of that byte.

- `bitarray_set`: first get the byte at the desired index and set a zero at the desired index with an inverted mask. If we want to set a 1, we fetch the value at that index from the array (supposedly a 1), otherwise don't do anything.
So: if we have the bitarray `0b00101101` and we wish to set a value of `1` (`true`) on the 2nd index, then
    - `bitarray->buf[bit_index / 8] = 0b00101101`
    -  `bitmask(1) = 0b00000010`
    - `~bitmask(1) = 0b11111101`

    Hence the first bitwise AND is `0b00101101 & 0b11111101` which yields `0b00101101`. Because the value is `true`, we bitwise OR `0b00101101 | 0b00000010` which yields `0b00101111`. 
    
    It is unclear why so many steps are needed but we will not touch the functionality here.

## Initial performance of the program on my laptop
`-s` yields 14 tiers, `-m` 15 tiers and `-l` 18 tiers. This is a simple test.

### Setting up Instruments.
We need to count events as they are occuring on the microarchitecture. It is believed that my [laptop](https://en.wikipedia.org/wiki/MacBook_Pro) has a Broadwell microarchitecture.

`system_profiler SPHardwareDataType` reveils:

    Hardware Overview:
      Model Name: MacBook Pro
      Model Identifier: MacBookPro12,1
      Processor Name: Intel Core i5
      Processor Speed: 2,9 GHz
      Number of Processors: 1
      Total Number of Cores: 2
      L2 Cache (per Core): 256 KB
      L3 Cache: 3 MB
      Hyper-Threading Technology: Enabled
      Memory: 16 GB

The microarchitecture documentation can then be collected from [here](https://download.01.org/perfmon/index/broadwell.html). Using the information about how [perf computes cache misses](https://stackoverflow.com/questions/55035313/how-does-linux-perf-calculate-the-cache-references-and-cache-misses-events), the most important events are:

| Event name | Hardware event name |
|- |- |
| cache-references | LONGEST_LAT_CACHE.REFERENCE |
| cache-misses | LONGEST_LAT_CACHE.MISS |
| L1-icache-load-misses | ICACHE.MISSES |
|  |  |
| L1-dcache-load-misses | L1D.REPLACEMENT |
| L1-dcache-loads | MEM_UOPS_RETIRED.ALL_LOADS |
| L1-dcache-stores | MEM_UOPS_RETIRED.ALL_STORES |  
|  |  |
| L1 Cache hits | MEM_LOAD_UOPS_RETIRED.L1_HIT |
| L1 Cache misses | MEM_LOAD_UOPS_RETIRED.L1_MISS |
| L2 Cache hits | MEM_LOAD_UOPS_RETIRED.L2_HIT |
| L2 Cache misses | MEM_LOAD_UOPS_RETIRED.L2_MISS |
| L3 Cache hits | MEM_LOAD_UOPS_RETIRED.L3_HIT |
| L3 Cache misses | MEM_LOAD_UOPS_RETIRED.L3_MISS |

Using the information about how [perf computes cache misses](https://stackoverflow.com/questions/55035313/how-does-linux-perf-calculate-the-cache-references-and-cache-misses-events) we can define the following formulas in Instruments:

- IPC (Instructions Per Cycle) = Instructions/Cycles
- Branch Misprediction Rate = MispredictedBranches/Branches
- L1 Cache Miss Rate = 100 x (L1CacheLoadMisses + L1CacheStoreMisses) / (L1CacheLoads + L1CacheStores)
= 100 x (MEM_LOAD_UOPS_RETIRED.L1_MISS + )


Using Instruments for profiling the `-l` option we can see:
- 2898.0ms  execution time
- 11.021.380.196 instructions
- 10.034.574.084 cycles (1.098 instr/cycle)
- 1.090.244.913 branches
- 765.107.680 taken branches (70%)

However, using Instruments we cannot measure all of the performance events at once, like one does with Valgrind. It is better to start development on the laptop and benchmark on the cluster.

## Performance measured on the cluster
We need to change the compiler to `gcc` as there is no `clang` on the cluster and we cannot seem to install it.

`-s` yields 14 tiers, `-m` 16 tiers and `-l` 19 tiers. This is a simple test.

### Performance tests
`valgrind --tool=cachegrind --branch-sim=yes ./everybit -l`

    ==11746== Cachegrind, a cache and branch-prediction profiler
    ==11746== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
    ==11746== Using Valgrind-3.14.0 and LibVEX; rerun with -h for copyright info
    ==11746== Command: ./everybit -l
    ==11746== 
    --11746-- warning: L3 cache found, using its data for the LL simulation.
    --11746-- warning: specified LL cache: line_size 64  assoc 20  total_size 26,214,400
    --11746-- warning: simulated LL cache: line_size 64  assoc 25  total_size 26,214,400
    ---- RESULTS ----
    Tier 0 (≈0B) completed in 0.001136s
    Tier 1 (≈0B) completed in 0.000086s
    Tier 2 (≈1B) completed in 0.000006s
    Tier 3 (≈1B) completed in 0.000010s
    Tier 4 (≈2B) completed in 0.000021s
    Tier 5 (≈4B) completed in 0.000047s
    Tier 6 (≈6B) completed in 0.000115s
    Tier 7 (≈11B) completed in 0.000291s
    Tier 8 (≈18B) completed in 0.000766s
    Tier 9 (≈29B) completed in 0.002002s
    Tier 10 (≈47B) completed in 0.005186s
    Tier 11 (≈76B) completed in 0.013554s
    Tier 12 (≈123B) completed in 0.035518s
    Tier 13 (≈199B) completed in 0.093152s
    Tier 14 (≈323B) completed in 0.230668s
    Tier 15 (≈522B) completed in 0.571084s
    Tier 16 (≈845B) exceeded 1.00s cutoff with time 1.496448s
    Succesfully completed tier: 15
    ---- END RESULTS ----
    ==11746== 
    ==11746== I   refs:      764,303,460
    ==11746== I1  misses:          1,522
    ==11746== LLi misses:          1,279
    ==11746== I1  miss rate:        0.00%
    ==11746== LLi miss rate:        0.00%
    ==11746== 
    ==11746== D   refs:      113,427,761  (85,055,183 rd   + 28,372,578 wr)
    ==11746== D1  misses:          4,544  (     3,706 rd   +        838 wr)
    ==11746== LLd misses:          3,230  (     2,459 rd   +        771 wr)
    ==11746== D1  miss rate:         0.0% (       0.0%     +        0.0%  )
    ==11746== LLd miss rate:         0.0% (       0.0%     +        0.0%  )
    ==11746== 
    ==11746== LL refs:             6,066  (     5,228 rd   +        838 wr)
    ==11746== LL misses:           4,509  (     3,738 rd   +        771 wr)
    ==11746== LL miss rate:          0.0% (       0.0%     +        0.0%  )
    ==11746== 
    ==11746== Branches:       28,415,706  (28,413,493 cond +      2,213 ind)
    ==11746== Mispredicts:        15,213  (    14,886 cond +        327 ind)
    ==11746== Mispred rate:          0.1% (       0.1%     +       14.8%   )

So it seems like there are very few cache misses and branch mispredictions, that is already really good! However, the performance is also impacted by measuring it. Now we can start looking at how changes influence the performance.

## Initial ideas
- Use a temporary value as we move each value around in the bitarray.
- Remove complete revolutions by checking first: is `modulo(-bit_right_amount, bit_length) == 0`?
- Inline some of the more simple functions, like `bitmask`, `bitarray_get_bit_sz` and `bitarray_get`.
- Use `__restrict__` as a keyword for the global array.

## Performance improvements
### Inlining
The functions `bitmask`, `bitarray_get_bit_sz` and `bitarray_get` were inlined but no performance increase was observed.

### Restricting
No performance increase when using keyword `restrict`.

### Checking
Check whether there is actually a modification needed 
