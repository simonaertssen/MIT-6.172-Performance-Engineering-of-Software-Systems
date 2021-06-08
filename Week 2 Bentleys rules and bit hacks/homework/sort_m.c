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


#include "./util.h"

extern void isort(data_t* begin, data_t* end);

static inline void merge_m(data_t* A, int p, int q, int r);
static inline void copy_m(data_t* source, data_t* dest, int n);

void sort_m(data_t* A, int p, int r) {
  assert(A);

  if (p < r) {
    if (r - p < 100) {
      isort(&(A[p]), &(A[r]));
    } else {
      int q = (p + r) / 2;
      sort_m(A, p, q);
      sort_m(A, q + 1, r);
      merge_m(A, p, q, r);
    }
  }
}

// A merge routine. Merges the sub-arrays A [p..q] and A [q + 1..r].
// Uses two arrays 'left' and 'right' in the merge operation.
static inline void merge_m(data_t* A, int p, int q, int r) {
  assert(A);
  assert(p <= q);
  assert((q + 1) <= r);
  int n1 = q - p + 1;
  int n2 = r - q;

  data_t* left = 0;
  mem_alloc(&left, n1 + 1);
  if (left == NULL) {
    mem_free(&left);
    return;
  }

  copy_m(&(A[p]), left, n1);
  left[n1] = UINT_MAX;

  /* Old merge:
  int i = 0;
  int j = 0;
  int yes = 0;

  // Here we access the arrays with pointers instead of indices
  for (int k = p; k <= r; k++) {
    if (j == n2) {
      yes = 1;
    }
    if (yes || *(left+i) <= *(A+q+1+j)) {
      *(A+k) = *(left+i); // Used to be A[k] = left[i];
      i++;
    } else {
      *(A+k) = *(A+q+1+j); // Used to be A[k] = right[j];
      j++;
    }
  }
  if (yes == 1) yes = 0;
  */

  // Here we make a merging algorithm based on the branchless minimum
  unsigned int * __restrict A_of_ptr = &(A[p]);
  unsigned int * __restrict leftptr = left;
  unsigned int * __restrict rghtptr = &(A[q+1]);

  while (n1 > 0 && n2 > 0) {
    long cmp = (*leftptr <= *rghtptr); // Store result of comparison
    long min = *rghtptr ^ ((*leftptr ^ *rghtptr) & -(cmp));

    // replace value with the minimum we found
    *A_of_ptr++ = min;

    // increase the pointer that had the minimum value
    leftptr += cmp; 
    n1 -= cmp;
    rghtptr += !cmp;
    n2 -= !cmp;
  }

  // empty the other sorted buffer
  while (n1 > 0) {
    *A_of_ptr++ = *leftptr++;
    n1--;  
  }

  mem_free(&left);
}

static inline void copy_m(data_t* source, data_t* dest, int n) {
  assert(dest);
  assert(source);

  for (int i = 0 ; i < n ; i++) {
    *(dest+i) = *(source+i); // Used to be dest[i] = source[i];
  }
}

