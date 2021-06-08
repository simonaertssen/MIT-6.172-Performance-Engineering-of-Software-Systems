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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Typedefs
typedef uint32_t data_t;

// Insertion sort, sorting the array between begin and end, inclusive
void isort(data_t* begin, data_t* end) {
  data_t* cur = begin + 1;
  while (cur <= end) {
    data_t val = *cur;
    data_t* index = cur - 1;

    while (index >= begin && *index > val) {
      *(index + 1) = *index;
      index--;
    }

    *(index + 1) = val;
    cur++;
  }
}


// Quicksort implementation
inline void swap(data_t* a, data_t* b) {
  data_t tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(data_t* A, int low, int high) {
  int flag  = 0;
  int first = low;
  int last  = high + 1;
  int pivot = A[low];

  while (flag == 0) {
    while (A[++first] < pivot) {
      if (first == high) flag = 1;
    }
    while (A[--last] > pivot) {
      if (last == low) flag = 1;
    }
    if (first >= last) {
      flag = 1;
    } else {
      swap(&A[first], &A[last]);
    }
  }
  swap(&A[low], &A[last]);
  return last;
}

void quicksort(data_t* A, int low, int high) {
  if (high <= low) {
    return;
  }

  int k = partition(A, low, high);
  quicksort(A, low, k - 1);
  quicksort(A, k + 1, high);
}