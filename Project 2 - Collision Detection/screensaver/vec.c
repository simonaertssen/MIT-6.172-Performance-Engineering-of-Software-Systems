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

#include "./vec.h"

#include <math.h>

#include "./line.h"

Vec Vec_make(const vec_dimension x, const vec_dimension y) {
  Vec vector;
  vector.x = x;
  vector.y = y;
  return vector;
}

Vec Vec_makeFromLine(struct Line line) {
  return Vec_subtract(line.p1, line.p2);
}

// ************************* Fundamental attributes **************************

vec_dimension Vec_length(Vec vector) {
  return hypot(vector.x, vector.y);
}

double Vec_argument(Vec vector) {
  return atan2(vector.y, vector.x);
}

// **************************** Related vectors ******************************

Vec Vec_normalize(Vec vector) {
  return Vec_divide(vector, Vec_length(vector));
}

Vec Vec_orthogonal(Vec vector) {
  return Vec_make(-vector.y, vector.x);
}

// ******************** Relationships with other vectors *********************

double Vec_angle(Vec vector1, Vec vector2) {
  return Vec_argument(vector1) - Vec_argument(vector2);
}

vec_dimension Vec_component(Vec vector1, Vec vector2) {
  return Vec_length(vector1) * cos(Vec_angle(vector1, vector2));
}

Vec Vec_projectOnto(Vec vector1, Vec vector2) {
  return Vec_multiply(Vec_normalize(vector2), Vec_component(vector1, vector2));
}

// ******************************* Arithmetic ********************************

bool Vec_equals(Vec lhs, Vec rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

Vec Vec_add(Vec lhs, Vec rhs) {
  return Vec_make(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec Vec_subtract(Vec lhs, Vec rhs) {
  return Vec_make(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec Vec_multiply(Vec vector, const double scalar) {
  return Vec_make(vector.x * scalar, vector.y * scalar);
}

Vec Vec_divide(Vec vector, const double scalar) {
  return Vec_make(vector.x / scalar, vector.y / scalar);
}

vec_dimension Vec_dotProduct(Vec lhs, Vec rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

vec_dimension Vec_crossProduct(Vec lhs, Vec rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}
