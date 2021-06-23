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

`bitarray_rotate`: assert whether the bounds for the subarray are actually contained within the bitarray. Remove full rotations by only shifting the number of bit shifts modulo the length of the subarray.

`bitarray_rotate_left`: for every bit shift that we need to perform, shift the whole subarray one bit to the left. Notice though, that this shifts to the right, due to the binary notation!

`bitarray_rotate_left_one`: 

## Initial performance of the program:

## Initial ideas
- Use a temporary value as we move each value around in the bitarray.
- Remove complete revolutions by checking first: is `modulo(-bit_right_amount, bit_length) == 0`?
