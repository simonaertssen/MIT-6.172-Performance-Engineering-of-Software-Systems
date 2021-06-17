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
#define _GNU_SOURCE
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "./bitarray.h"
#include "./ktiming.h"
#include "./tests.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ******************************* Prototypes *******************************

// Creates a new bit array in test_bitarray by parsing a string of 0s
// and 1s.  For instance, "0101011011" is a suitable argument.
void testutil_frmstr(const char* const bitstring);

// Rotates test_bitarray in place.
// Requires that test_bitarray is not NULL.
void testutil_rotate(const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_shift_amount);

// Checks that the rotation is valid given the size of test_bitarray.
// Causes a test suite failure if the input is invalid.
void testutil_require_valid_input(const size_t bit_offset,
                                  const size_t bit_length,
                                  const ssize_t bit_right_shift_amount,
                                  const char* const func_name,
                                  const int line);

// Creates a new bit array in test_bitarray of the specified size and
// fills it with random data based on the seed given.  For a given seed number,
// the pseudorandom data will be the same (at least on the same glibc
// implementation).
static void testutil_newrand(const size_t bit_sz, const unsigned int seed);

// Prints a string representation of a bit array.
static void bitarray_fprint(FILE* const stream,
                            const bitarray_t* const bitarray);

// Verifies that test_bitarray has the expected content.
// Outputs FAIL or PASS as appropriate.
// Note: You can call this function directly, but it's much cleaner to use the
// testutil_expect macro instead.
// Requires that test_bitarray is not NULL.
static void testutil_expect_internal(const char* const bitstring,
                                     const char* const func_name,
                                     const int line);

// Converts a character into a boolean.  The character '1' converts to true;
// the character '0' converts to false.
static bool boolfromchar(const char c);

// Retrieves a char* argument from a buffer in strtok.
char* next_arg_char();


// ******************************** Globals *********************************
// Some global variables make it easier to run individual tests.

// The bit array currently under test.
static bitarray_t* test_bitarray = NULL;

// Whether or not tests should be verbose.
static bool test_verbose = false;


// ********************************* Macros *********************************

// Marks a test as successful, outputting its name and line.
#define TEST_PASS() TEST_PASS_WITH_NAME(__func__, __LINE__)

// Marks a test as successful, outputting the specified name and line.
#define TEST_PASS_WITH_NAME(name, line)          \
  fprintf(stderr, " --> %s at line %d: PASS\n", (name), (line))

// Marks a test as unsuccessful, outputting its name, line, and the specified
// failure message.
//
// Use this macro just like you would call printf.
#define TEST_FAIL(failure_msg, args...)          \
  TEST_FAIL_WITH_NAME(__func__, __LINE__, failure_msg, ##args)

// Marks a test as unsuccessful, outputting the specified name, line, and the
// failure message.
//
// Use this macro just like you would call printf.
#define TEST_FAIL_WITH_NAME(name, line, failure_msg, args...)    \
  do {                \
    fprintf(stderr, " --> %s at line %d: FAIL\n    Reason:", \
      (name), (line));        \
    fprintf(stderr, (failure_msg), ##args);      \
    fprintf(stderr, "\n");          \
  } while (0)

// Calls testutil_expect_internal with the current function and line
// number.
// Requires that test_bitarray is not NULL.
#define testutil_expect(bitstring)        \
  testutil_expect_internal((bitstring), __func__, __LINE__)

// Retrieves an integer from the strtok buffer.
#define NEXT_ARG_LONG() atol(strtok(NULL, " "))

// ******************************* Functions ********************************

static void testutil_newrand(const size_t bit_sz, const unsigned int seed) {
  // If we somehow managed to avoid freeing test_bitarray after a previous
  // test, go free it now.
  if (test_bitarray != NULL) {
    bitarray_free(test_bitarray);
  }

  test_bitarray = bitarray_new(bit_sz);
  assert(test_bitarray != NULL);

  // Reseed the RNG with whatever we were passed; this ensures that we can
  // repeat the test deterministically by specifying the same seed.
  srand(seed);

  bitarray_randfill(test_bitarray);

  // If we were asked to be verbose, go ahead and show the bit array and
  // the random seed.
  if (test_verbose) {
    bitarray_fprint(stdout, test_bitarray);
    fprintf(stdout, " newrand sz=%zu, seed=%u\n",
            bit_sz, seed);
  }
}

void testutil_frmstr(const char* const bitstring) {
  const size_t bitstring_length = strlen(bitstring);

  // If we somehow managed to avoid freeing test_bitarray after a previous
  // test, go free it now.
  if (test_bitarray != NULL) {
    bitarray_free(test_bitarray);
  }

  test_bitarray = bitarray_new(bitstring_length);
  assert(test_bitarray != NULL);

  bool current_bit;
  for (size_t i = 0; i < bitstring_length; i++) {
    current_bit = boolfromchar(bitstring[i]);
    bitarray_set(test_bitarray, i, current_bit);
  }
  bitarray_fprint(stdout, test_bitarray);
  if (test_verbose) {
    fprintf(stdout, " newstr lit=%s\n", bitstring);
    testutil_expect(bitstring);
  }
}

static void bitarray_fprint(FILE* const stream,
                            const bitarray_t* const bitarray) {
  for (size_t i = 0; i < bitarray_get_bit_sz(bitarray); i++) {
    fprintf(stream, "%d", bitarray_get(bitarray, i) ? 1 : 0);
  }
}

static void testutil_expect_internal(const char* bitstring,
                                     const char* const func_name,
                                     const int line) {
  // The reason why the test fails.  If the test passes, this will stay
  // NULL.
  const char* bad = NULL;

  assert(test_bitarray != NULL);

  // Check the length of the bit array under test.
  const size_t bitstring_length = strlen(bitstring);
  if (bitstring_length != bitarray_get_bit_sz(test_bitarray)) {
    bad = "bitarray size";
  }

  // Check the content.
  for (size_t i = 0; i < bitstring_length; i++) {
    if (bitarray_get(test_bitarray, i) != boolfromchar(bitstring[i])) {
      bad = "bitarray content";
    }
  }

  // Obtain a string for the actual bitstring.
  const size_t actual_bitstring_length = bitarray_get_bit_sz(test_bitarray);
  char* actual_bitstring = calloc(sizeof(char), bitstring_length + 1);
  for (size_t i = 0; i < actual_bitstring_length; i++) {
    if (bitarray_get(test_bitarray, i)) {
      actual_bitstring[i] = '1';
    } else {
      actual_bitstring[i] = '0';
    }
  }

  if (bad != NULL) {
    bitarray_fprint(stdout, test_bitarray);
    fprintf(stdout, " expect bits=%s \n", bitstring);
    TEST_FAIL_WITH_NAME(func_name, line, " Incorrect %s.\n    Expected: %s\n    Actual:   %s",
                        bad, bitstring, actual_bitstring);
  } else {
    TEST_PASS_WITH_NAME(func_name, line);
  }
  free(actual_bitstring);
}

void testutil_rotate(const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_shift_amount) {
  assert(test_bitarray != NULL);
  bitarray_rotate(test_bitarray, bit_offset, bit_length, bit_right_shift_amount);
  if (test_verbose) {
    bitarray_fprint(stdout, test_bitarray);
    fprintf(stdout, " rotate off=%zu, len=%zu, amnt=%zd\n",
            bit_offset, bit_length, bit_right_shift_amount);
  }
}

void testutil_require_valid_input(const size_t bit_offset,
                                  const size_t bit_length,
                                  const ssize_t bit_right_shift_amount,
                                  const char* const func_name,
                                  const int line) {
  size_t bitarray_length = bitarray_get_bit_sz(test_bitarray);
  if (bit_offset >= bitarray_length || bit_length > bitarray_length ||
      bit_offset + bit_length > bitarray_length) {
    // invalid input
    TEST_FAIL_WITH_NAME(func_name, line, " TEST SUITE ERROR - " \
                        "bit_offset + bit_length > bitarray_length");
  }
}

// Precomputed array of fibonacci numbers
const int FIB_SIZE = 53;
const double fibs[FIB_SIZE] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073, 4807526976, 7778742049, 12586269025, 20365011074, 32951280099, 53316291173, 86267571272};

int timed_rotation(const double time_limit_seconds) {
  // We're going to be doing a bunch of rotations; we probably shouldn't
  // let the user see all the verbose output.
  test_verbose = false;

  // Continue until the rotation exceeds time_limits_seconds
  int tier_num = 0;
  while(tier_num + 3 < FIB_SIZE){
    const size_t bit_offset             = fibs[tier_num];
    const size_t bit_right_shift_amount = fibs[tier_num+1];
    const size_t bit_length             = fibs[tier_num+2];
    const size_t bit_sz                 = fibs[tier_num+3];
    assert(bit_sz > bit_length);
    assert(bit_length > bit_right_shift_amount);
    assert(bit_right_shift_amount > bit_offset);
    assert(bit_sz > bit_offset + bit_length);

    // Initialize a new bit_array
    testutil_newrand(bit_sz, 6172);
 
    // Time the duration of a rotation
    const clockmark_t start_time = ktiming_getmark();
    testutil_rotate(bit_offset, bit_length, bit_right_shift_amount);
    const clockmark_t end_time = ktiming_getmark();
    double diff_seconds = ktiming_diff_usec(&start_time, &end_time) / 1000000000.0;

    //char *str_size = NULL;
    char buf[20];
    if (bit_length < 8*1024){
        sprintf(buf, "%luB", bit_length / 8);
    } else if (bit_length < 8 * 1024 * 1024){
        sprintf(buf, "%luKB", bit_length / (8 * 1024));
    } else if (bit_length < 8UL * 1024 * 1024 * 1024){
        sprintf(buf, "%luMB", bit_length / (8 * 1024 * 1024));
    } else {
        sprintf(buf, "%luGB", bit_length / (8UL * 1024 * 1024 * 1024));
    }
    if (diff_seconds < time_limit_seconds){
      printf("Tier %d (≈%s) completed in " ANSI_COLOR_GREEN "%.6fs" ANSI_COLOR_RESET "\n",
        tier_num, buf, diff_seconds);
      tier_num++;
    } else {
      printf("Tier %d (≈%s) exceeded %.2fs cutoff with time" ANSI_COLOR_RED " %.6fs" ANSI_COLOR_RESET "\n",
         tier_num, buf, time_limit_seconds, diff_seconds);
      // Return the last tier that was succesful.
      return tier_num - 1;
    }
  }

  // Return the last tier that was succesful.
  return tier_num - 1;
}

static bool boolfromchar(const char c) {
  assert(c == '0' || c == '1');
  return c == '1';
}

char* next_arg_char() {
  char* buf = strtok(NULL, " ");
  char* eol = NULL;
  if ((eol = strchr(buf, '\n')) != NULL) {
    *eol = '\0';
  }
  return buf;
}

void parse_and_run_tests(const char* filename, int selected_test) {
  test_verbose = false;
  fprintf(stderr, "Testing file %s.\n", filename);
  FILE* f = fopen(filename, "r");

  char* buf = NULL;
  size_t bufsize = 0;
  int test = -1;
  int line = 0;
  bool ready_to_run = false;
  if (f == NULL) {
    fprintf(stderr, "Error opening file.\n");
    return;
  }
  while (getline(&buf, &bufsize, f) != -1) {
    line++;
    char* token = strtok(buf, " ");
    switch (token[0]) {
    case '\n':
    case '#':
      continue;
    case 't':
      test = (int) NEXT_ARG_LONG();
      ready_to_run = (test == selected_test || selected_test == -1);
      if (!ready_to_run) {
        continue;
      }

      fprintf(stderr, "\nRunning test #%d...\n", test);
      break;
    case 'n':
      if (!ready_to_run) {
        continue;
      }
      testutil_frmstr(next_arg_char());
      break;
    case 'e':
      if (!ready_to_run) {
        continue;
      }
      {
        char* expected = next_arg_char();
        testutil_expect_internal(expected, filename, line);
      }
      break;
    case 'r':
      if (!ready_to_run) {
        continue;
      }
      {
        size_t offset = (size_t) NEXT_ARG_LONG();
        size_t length = (size_t) NEXT_ARG_LONG();
        ssize_t amount = (ssize_t) NEXT_ARG_LONG();
        testutil_require_valid_input(offset, length, amount, filename, line);
        testutil_rotate(offset, length, amount);
      }
      break;
    default:
      fprintf(stderr, "Unknown command %s", buf);
    }
  }
  free(buf);

  fprintf(stderr, "Done testing file %s.\n", filename);
}

// Local Variables:
// mode: C
// fill-column: 100
// c-file-style: "k&r"
// c-basic-offset: 2
// indent-tabs-mode: nil
// tab-width: 2
// End:
