/**
 * Copyright (c) 2012 the Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cilk/cilk.h>


#include "./fasttime.h"
#include "./line.h"
#include "./line_demo.h"
#include "./cilktool.h"

// The PROFILE_BUILD preprocessor define is used to indicate we are building for
// profiling, so don't include any graphics or Cilk functions.
#ifndef PROFILE_BUILD
#include "./graphic_stuff.h"
#endif
static char* DEFAULT_INPUT_FILE_PATH = "input/mit.in";
static char* input_file_path;

// For non-graphic version
void lineMain(LineDemo *lineDemo) {
  // Loop for updating line movement simulation
  while (true) {
    if (!LineDemo_update(lineDemo)) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int optchar;
#ifndef PROFILE_BUILD
  bool graphicDemoFlag = false;
#endif
  unsigned int numFrames = 1;
  extern int optind;

  // Process command line options.
  while ((optchar = getopt(argc, argv, "gi")) != -1) {
    switch (optchar) {
      case 'g':
#ifndef PROFILE_BUILD
        graphicDemoFlag = true;
#endif
        break;
      default:
        printf("Ignoring unrecognized option: %c\n", optchar);
        continue;
    }
  }

  // Shift remaining arguments over.
  int remaining_args = argc - optind;
  for (int i = 1; i <= remaining_args; i++) {
    argv[i] = argv[i + optind - 1];
  }

  // Check to make sure number of arguments is correct.
  if (remaining_args < 1) {
    printf("Usage: %s [-g] <numFrames> [inputfile]\n", argv[0]);
    printf("  -g : show graphics\n");
    exit(-1);
  }

  numFrames = atoi(argv[1]);
  printf("Number of frames = %u\n", numFrames);

  if (remaining_args > 1) {
    input_file_path = argv[2];
  } else {
    input_file_path = DEFAULT_INPUT_FILE_PATH;
  }
  printf("Input file path is: %s\n", input_file_path);

  // Create and initialize the Line simulation environment.
  LineDemo *lineDemo = LineDemo_new();
  LineDemo_setInputFile(input_file_path);
  LineDemo_initLine(lineDemo);
  LineDemo_setNumFrames(lineDemo, numFrames);

  const fasttime_t start_time = gettime();

#ifndef PROFILE_BUILD
  // Run demo.
  if (graphicDemoFlag) {
    graphicMain(argc, argv, lineDemo, false);
  } else {
    lineMain(lineDemo);
  }
#else
  lineMain(lineDemo);
#endif

  const fasttime_t end_time = gettime();

  // Output results.
  printf("---- RESULTS ----\n");
  printf("Elapsed execution time: %fs\n",
         tdiff(start_time, end_time));
  printf("%u Line-Wall Collisions\n",
         LineDemo_getNumLineWallCollisions(lineDemo));
  printf("%u Line-Line Collisions\n",
         LineDemo_getNumLineLineCollisions(lineDemo));
  printf("---- END RESULTS ----\n");

  // delete objects
  LineDemo_delete(lineDemo);
#ifdef CILKSCALE
  print_total();
#endif

  return 0;
}
