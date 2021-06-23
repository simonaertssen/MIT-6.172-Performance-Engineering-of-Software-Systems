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

## Initial performance of the program:
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

|  	|  	|
|-	|-	|
|  	|  	|


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


## Initial ideas
- Use a temporary value as we move each value around in the bitarray.
- Remove complete revolutions by checking first: is `modulo(-bit_right_amount, bit_length) == 0`?
- Inline some of the more simple functions, like `bitmask`, `bitarray_get_bit_sz` and `bitarray_get`.
