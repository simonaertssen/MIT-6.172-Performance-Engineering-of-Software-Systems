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

// LineDemo.h -- main driver for the line simulation
#ifndef LINEDEMO_H_
#define LINEDEMO_H_

#include "./line.h"
#include "./collision_world.h"

struct LineDemo {
  // Iteration counter
  unsigned int count;

  // Number of frames to compute
  unsigned int numFrames;

  // Objects for line simulation
  CollisionWorld* collisionWorld;
};
typedef struct LineDemo LineDemo;

LineDemo* LineDemo_new();
void LineDemo_delete(LineDemo* lineDemo);

// Add lines for line simulation at beginning.
void LineDemo_createLines(LineDemo* lineDemo);

// Set number of frames to compute.
void LineDemo_setNumFrames(LineDemo* lineDemo, const unsigned int numFrames);

// Initialize line simulation.
void LineDemo_initLine(LineDemo* lineDemo);

// Get ith line.
Line* LineDemo_getLine(LineDemo* lineDemo, const unsigned int index);

// Get num of lines.
unsigned int LineDemo_getNumOfLines(LineDemo* lineDemo);

// Get number of line-wall collisions.
unsigned int LineDemo_getNumLineWallCollisions(LineDemo* lineDemo);

// Get number of line-line collisions.
unsigned int LineDemo_getNumLineLineCollisions(LineDemo* lineDemo);

// Line simulation update function.
bool LineDemo_update(LineDemo* lineDemo);

void LineDemo_setInputFile(char* input_file_path);

#endif  // LINEDEMO_H_
