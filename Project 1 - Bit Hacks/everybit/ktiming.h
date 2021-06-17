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

// This is a Linux kernel-assisted timing library; it provides high-precision
// time measurements for the execution time of your algorithms.
//
// It also provides timing functionality on other platforms such as
// <a href="http://cygwin.com/">Cygwin</a> and
// <a href="https://en.wikipedia.org/wiki/Darwin_(operating_system)">Darwin</a>
// for portability, but the meaning of the time reported may vary.  On Linux,
// the library measures CPU time; on Darwin and Cygwin, however, it will report
// the wall time.  For example, timing sleep(1) on Linux will return a
// number very close to 0; on Darwin or Cygwin, it will return a number very
// close to 1.

#ifndef _KTIMING_H_
#define _KTIMING_H_

#include <stdint.h>


// ********************************* Types **********************************

// A clock time.
typedef uint64_t clockmark_t;


// ******************************* Prototypes *******************************

// Returns the difference between two clockmark_t in nanoseconds.  In
// particular, returns *end - *start.
//
// Warning: Although the function is called ktiming_diff_usec, it returns a
// value in nanoseconds, not microseconds!
uint64_t ktiming_diff_usec(const clockmark_t* const start,
                           const clockmark_t* const end);

// Returns the difference between two clockmark_t in seconds.  In
// particular, returns *end - *start.
float ktiming_diff_sec(const clockmark_t* const start,
                       const clockmark_t* const end);

// Gets the current clock time.
clockmark_t ktiming_getmark();

#endif  // _KTIMING_H_
