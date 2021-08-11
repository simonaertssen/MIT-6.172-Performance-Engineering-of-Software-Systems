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

// Two-dimensional lines
#ifndef LINE_H_
#define LINE_H_

#include "./graphic_stuff.h"
#include "./vec.h"

// Lines' coordinates are stored in a box with these bounds
// We choose box coordinates in [.5, 1) to simulate fixed
// point floating point accuracy to mitigate issues with
// associativity of coordinate updates
#define BOX_XMIN .5
#define BOX_XMAX 1
#define BOX_YMIN .5
#define BOX_YMAX 1

// Graphics are displayed in a box of this size.
#define WINDOW_WIDTH 1180
#define WINDOW_HEIGHT 800

typedef double window_dimension;
typedef vec_dimension box_dimension;

// The allowable colors for a line.
typedef enum {
  RED = 0,
  GRAY = 1
} Color;

// A two-dimensional line.
struct Line {
  Vec p1;  // One endpoint of the line.
  Vec p2;  // The other endpoint of the line.

  // The line's current velocity, in units of pixels per time step.
  Vec velocity;

  Color color;  // The line's color.

  unsigned int id;  // Unique line ID.
};
typedef struct Line Line;

// Compares the lines by line ID.
// -1 <=> line1 ordered before line2
//  0 <=> line1 ordered the same as line2
//  1 <=> line1 ordered after line2
static inline int compareLines(Line *line1, Line *line2) {
  if (line1->id < line2->id) {
    return -1;
  } else if (line1->id == line2->id) {
    return 0;
  } else {
    return 1;
  }
}

// Convert graphical window coordinates to box coordinates.
static inline void windowToBox(box_dimension *xout, box_dimension *yout,
                               window_dimension x, window_dimension y) {
  *xout = x / WINDOW_WIDTH * ((double) BOX_XMAX - BOX_XMIN) + BOX_XMIN;
  *yout = y / WINDOW_HEIGHT * ((double) BOX_YMAX - BOX_YMIN) + BOX_YMIN;
}

// Convert box coordinates to graphical window coordinates.
static inline void boxToWindow(window_dimension *xout, window_dimension *yout,
                               box_dimension x, box_dimension y) {
  *xout = (x - BOX_XMIN) / ((double) BOX_XMAX - BOX_XMIN) * WINDOW_WIDTH;
  *yout = (y - BOX_YMIN) / ((double) BOX_YMAX - BOX_YMIN) * WINDOW_HEIGHT;
}

// Convert graphical window velocity to box velocity.
static inline void velocityWindowToBox(box_dimension *xout, box_dimension *yout,
                                       window_dimension x, window_dimension y) {
  *xout = x / WINDOW_WIDTH * ((double) BOX_XMAX - BOX_XMIN);
  *yout = y / WINDOW_HEIGHT * ((double) BOX_YMAX - BOX_YMIN);
}

#endif  // LINE_H_
