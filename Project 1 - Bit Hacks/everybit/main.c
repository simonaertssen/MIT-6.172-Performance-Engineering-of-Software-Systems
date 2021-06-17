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

// This test harness which allows both functional testing (i.e., testing for
// correctness) and performance testing (i.e., testing for speed).

// We need _POSIX_C_SOURCE >= 2 to use getopt.
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "./tests.h"


// ******************************* Prototypes *******************************

void print_usage(const char* const argv_0);


// ******************************* Functions ********************************

int main(int argc, char** argv) {
  int retval = EXIT_SUCCESS;

  // Parse options.
  char optchar;
  opterr = 0;
  int selected_test = -1;
  while ((optchar = getopt(argc, argv, "n:t:sml")) != -1) {
    switch (optchar) {
    case 'n':
      selected_test = atoi(optarg);
      break;
    case 't':
      // -t file runs functional tests in the provided file
      parse_and_run_tests(optarg, selected_test);
      retval = EXIT_SUCCESS;
      goto cleanup;
    case 's':
      // -s runs the short rotation performance test.
      printf("---- RESULTS ----\n");
      printf("Succesfully completed tier: %d\n",
             timed_rotation(0.01));
      printf("---- END RESULTS ----\n");
      retval = EXIT_SUCCESS;
      goto cleanup;
    case 'm':
      // -m runs the medium rotation performance test.
      printf("---- RESULTS ----\n");
      printf("Succesfully completed tier: %d\n",
             timed_rotation(0.1));
      printf("---- END RESULTS ----\n");
      retval = EXIT_SUCCESS;
      goto cleanup;
    case 'l':
      // -l runs the large rotation performance test.
      printf("---- RESULTS ----\n");
      printf("Succesfully completed tier: %d\n",
             timed_rotation(1.0));
      printf("---- END RESULTS ----\n");
      retval = EXIT_SUCCESS;
      goto cleanup;
    }
  }

  // If we made it here, we couldn't find any arguments to tell us what to
  // do.  Go and tell the user how to actually run the program.
  print_usage(argv[0]);
  retval = EXIT_SUCCESS;

cleanup:
  return retval;
}

void print_usage(const char* const argv_0) {
  fprintf(stderr, "usage: %s\n"
          "\t -s Run a sample small (0.01s) rotation operation\n"
          "\t -m Run a sample medium (0.1s) rotation operation\n"
          "\t -l Run a sample large (1s) rotation operation\n"
          "\t    (note: the provided -[s/m/l] options only test performance and NOT correctness.)\n"
          "\t -t tests/default\tRun alltests in the testfile tests/default\n"
          "\t -n 1 -t tests/default\tRun test 1 in the testfile tests/default\n",
          argv_0);
}
