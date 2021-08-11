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

// Simple 2D vector library
#ifndef VEC_H_
#define VEC_H_

#include <stdbool.h>

typedef double vec_dimension;

// Forward definition of Line to avoid needing to circularly include Line.h
struct Line;

// A two-dimensional vector.
struct Vec {
  vec_dimension x;  // The x-coordinate of the vector.
  vec_dimension y;  // The y-coordinate of the vector.
};
typedef struct Vec Vec;

// Returns a vector with the specified x and y coordinates.
Vec Vec_make(const vec_dimension x, const vec_dimension y);

// Returns a vector parallel to the provided Line.  The direction of the
// vector is unspecified.
Vec Vec_makeFromLine(struct Line line);

// Returns the magnitude of the vector.
vec_dimension Vec_length(Vec vector);

// Returns the argument of the vector - that is, the angle it makes with the
// positive x axis.  Units are radians.
double Vec_argument(Vec vector);

// Returns a unit vector parallel to the vector.
Vec Vec_normalize(Vec vector);

// Returns a vector identical in magnitude and perpendicular to the vector.
Vec Vec_orthogonal(Vec vector);

// Computes the angle between vector1 and vector2.
double Vec_angle(Vec vector1, Vec vector2);

// Computes the scalar component of vector1 onto vector2.
vec_dimension Vec_component(Vec vector1, Vec vector2);

// Returns the vector projection of vector1 onto vector2.
Vec Vec_projectOnto(Vec vector1, Vec vector2);

bool Vec_equals(Vec lhs, Vec rhs);
Vec Vec_add(Vec lhs, Vec rhs);
Vec Vec_subtract(Vec lhs, Vec rhs);
Vec Vec_multiply(Vec vector, const double scalar);
Vec Vec_divide(Vec vector, const double scalar);

// Computes the dot product of two vectors.
vec_dimension Vec_dotProduct(Vec lhs, Vec rhs);

// Computes the magnitude of the cross product of two vectors.
vec_dimension Vec_crossProduct(Vec lhs, Vec rhs);

#endif  // VEC_H_
