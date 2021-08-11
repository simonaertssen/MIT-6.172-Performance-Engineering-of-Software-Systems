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

#ifndef INTERSECTIONDETECTION_H_
#define INTERSECTIONDETECTION_H_

#include "./line.h"
#include "./vec.h"

typedef enum {
  NO_INTERSECTION,
  L1_WITH_L2,
  L2_WITH_L1,
  ALREADY_INTERSECTED
} IntersectionType;

// Detect if line l1 and l2 will be intersected in the next time step.
// Precondition: compareLines(l1, l2) < 0 must be true.
IntersectionType intersect(Line *l1, Line *l2, double time);

// Check if a point is in the parallelogram.
bool pointInParallelogram(Vec point, Vec p1, Vec p2, Vec p3, Vec p4);

// Check if two lines intersect.
bool intersectLines(Vec p1, Vec p2, Vec p3, Vec p4);

// Check the direction of two lines (pi, pj) and (pi, pk).
double direction(Vec pi, Vec pj, Vec pk);

// Check if a point pk is in the line segment (pi, pj).
bool onSegment(Vec pi, Vec pj, Vec pk);

// Calculate the cross product.
double crossProduct(double x1, double y1, double x2, double y2);

// Obtain the intersection point for two intersecting line segments.
Vec getIntersectionPoint(Vec p1, Vec p2, Vec p3, Vec p4);

#endif  // INTERSECTIONDETECTION_H_
