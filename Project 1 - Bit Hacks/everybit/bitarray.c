/**
 * Copyright (c) 2012 MIT License by 6.172 Staff
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

 // Implements the ADT specified in bitarray.h as a packed array of bits; a bit
 // array containing bit_sz bits will consume roughly bit_sz/8 bytes of
 // memory.


#include "./bitarray.h"
#include "./tests.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>


// ********************************* Types **********************************

// Concrete data type representing an array of bits.
struct bitarray {
  // The number of bits represented by this bit array.
  // Need not be divisible by 8.
  size_t bit_sz;

  // The underlying memory buffer that stores the bits in
  // packed form (8 per byte).
  char* buf;
};


// ******************** Prototypes for static functions *********************

// Rotates a subarray left by an arbitrary number of bits.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
// bit_left_amount is the number of places to rotate the
//                    subarray left
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length,
  const size_t bit_left_amount) __attribute__((unused)); // Added unused flag, as compiler flag could not pick it up

// Rotates a subarray left by one bit.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left_one(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length);


// ****************** Prototypes for optimised functions *******************

//
// Rotates subarray by moving each bit to its specified location
// directly. See the README for more information.
// 
// Keep the temporay value of the bit along with its location between every
// iteration. Move the bit stored in this temporary index to its new location.
// Space complexity: O(1).
// Time complexity: O(bit_length).
// 
// The subarray spans the half-open interval [bit_offset, bit_offset +
// bit_length). That is, the start is inclusive, but the end is exclusive.
// 
// NOTE: Although constant auxillary space is used, the memory accesses are
// scattered, which can adversely impact caching.
static void bitarray_rotate_cyclic(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length,
  const ssize_t bit_right_amount);

// Produces a mask which, when ANDed with a byte, retains only the
// bit_index th byte.
//
// Example: bitmask(5) produces the byte 0b00100000.
//
// (Note that here the index is counted from right
// to left, which is different from how we represent bitarrays in the
// tests.  This function is only used by bitarray_get and bitarray_set,
// however, so as long as you always use bitarray_get and bitarray_set
// to access bits in your bitarray, this reverse representation should
// not matter.
static char bitmask(const size_t bit_index);


// ******************************* Functions ********************************

bitarray_t* bitarray_new(const size_t bit_sz) {
  // Allocate an underlying buffer of ceil(bit_sz/8) bytes.
  char* const buf = calloc(1, (bit_sz + 7) / 8);
  if (buf == NULL) {
    return NULL;
  }

  // Allocate space for the struct.
  bitarray_t* const bitarray = malloc(sizeof(struct bitarray));
  if (bitarray == NULL) {
    free(buf);
    return NULL;
  }

  bitarray->buf = buf;
  bitarray->bit_sz = bit_sz;
  return bitarray;
}

void bitarray_free(bitarray_t* const bitarray) {
  if (bitarray == NULL) {
    return;
  }
  free(bitarray->buf);
  bitarray->buf = NULL;
  free(bitarray);
}

size_t bitarray_get_bit_sz(const bitarray_t* const bitarray) {
  return bitarray->bit_sz;
}

bool bitarray_get(const bitarray_t* const bitarray, const size_t bit_index) {
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to get the nth
  // bit, we want to look at the (n mod 8)th bit of the (floor(n/8)th)
  // byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to produce either a zero byte (if the bit was 0) or a nonzero byte
  // (if it wasn't).  Finally, we convert that to a boolean.
  return (bitarray->buf[bit_index / 8] & bitmask(bit_index)) ? true : false;
}

void bitarray_set(bitarray_t* const bitarray,
  const size_t bit_index,
  const bool value) {
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to set the nth
  // bit, we want to set the (n mod 8)th bit of the (floor(n/8)th) byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to clear out the bit we're about to set.  We bitwise-or the result
  // with a byte that has either a 1 or a 0 in the correct place.
  bitarray->buf[bit_index / 8] = (bitarray->buf[bit_index / 8] & ~bitmask(bit_index)) | (value ? bitmask(bit_index) : 0);
}

void bitarray_randfill(bitarray_t* const bitarray) {
  int32_t* ptr = (int32_t*)bitarray->buf;
  for (int64_t i = 0; i < bitarray->bit_sz / 32 + 1; i++) {
    ptr[i] = rand();
  }
}

void bitarray_rotate(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length,
  const ssize_t bit_right_amount) {
  assert(bit_offset + bit_length <= bitarray->bit_sz);
  if (bit_length == 0) return;

  // Convert a rotate left or right to a left rotate only:
  // bitarray_rotate_left(bitarray, bit_offset, bit_length, modulo(-bit_right_amount, bit_length));

  // Check whether we actually need to shift the array or not: eliminate
  // multiple full rotations.
  size_t k = modulo(bit_right_amount, bit_length);
  if (k == 0) return;

  // cyclic rotation: prevent moving these bits one by one, but move immediately to the right place
  bitarray_rotate_cyclic(bitarray, bit_offset, bit_length, k);
}

static void bitarray_rotate_left(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length,
  const size_t bit_left_amount) {
  for (size_t i = 0; i < bit_left_amount; i++) {
    bitarray_rotate_left_one(bitarray, bit_offset, bit_length);
  }
}

static void bitarray_rotate_left_one(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length) {
  // Grab the first bit in the range, shift everything left by one, and
  // then stick the first bit at the end.
  const bool first_bit = bitarray_get(bitarray, bit_offset);
  size_t i;
  for (i = bit_offset; i + 1 < bit_offset + bit_length; i++) {
    bitarray_set(bitarray, i, bitarray_get(bitarray, i + 1));
  }
  bitarray_set(bitarray, i, first_bit);
}

size_t modulo(const ssize_t n, const size_t m) {
  const ssize_t signed_m = (ssize_t)m;
  assert(signed_m > 0);
  const ssize_t result = ((n % signed_m) + signed_m) % signed_m;
  assert(result >= 0);
  return (size_t)result;
}

static char bitmask(const size_t bit_index) {
  return 1 << (bit_index % 8);
}


// ********************** Optimised functions ***********************

static void bitarray_rotate_cyclic(bitarray_t* const bitarray,
  const size_t bit_offset,
  const size_t bit_length,
  const ssize_t bit_right_amount) {
  assert(bit_offset + bit_length <= bitarray->bit_sz);
  if (bit_length == 0) return;

  bool PRINT = false;

  if (PRINT) {
    printf("      , Array now: ");
    bitarray_fprint(stdout, bitarray);
  }

  size_t prv = bit_offset;                                                            // index of previous element
  size_t nxt = bit_offset + modulo(prv + bit_right_amount - bit_offset, bit_length);  // index of next element

  bool cycles = !(bit_length % bit_right_amount); // If they divide then we have cycles
  size_t nxt_first_ever_value = nxt;  // save first ever value so we can see when we encounter it again
  size_t prv_first_ever_value = prv;  // save first ever value so we can see when we encounter it again
  size_t prv_first_cycle_value = prv; // save first cycle value so we can see when we encounter it again
  bool already_cycled = false;

  bool x = bitarray_get(bitarray, prv); // previous value in array
  bool y = bitarray_get(bitarray, nxt); // next value in array

  if (PRINT) printf(", prv = %zu, nxt = %zu, x = %d, y = %d \n", prv, nxt, x, y);

  for (size_t i = 0; i < bit_length; i++) {
    bitarray_set(bitarray, nxt, x);     // replace next value with previous one
    x = y;                              // replace value 'pointers'
    prv = nxt;

    // if (cycles && prv == prv_first_cycle_value) {
    //   already_cycled = true;
    //   prv_first_cycle_value++;
    //   x = bitarray_get(bitarray, ++prv);
    // }

    // if (cycles && already_cycled && (prv == prv_first_ever_value || prv == nxt_first_ever_value)) break;

    nxt = bit_offset + modulo(prv + bit_right_amount - bit_offset, bit_length);       // mod with respect to begin of subarray
    y = bitarray_get(bitarray, nxt);

    if (PRINT) {
      printf("i = %.2zu, Array now: ", i);
      bitarray_fprint(stdout, bitarray);
      printf(", prv = %zu, nxt = %zu, x = %d, y = %d \n", prv, nxt, x, y);
    }
  }
}