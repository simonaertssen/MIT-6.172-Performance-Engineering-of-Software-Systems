/**
 * collision_world.h -- detect and handle line segment intersections
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

#ifndef COLLISIONWORLD_H_
#define COLLISIONWORLD_H_

#include "./line.h"
#include "./intersection_detection.h"

struct CollisionWorld {
  // Time step used for simulation
  double timeStep;

  // Container that holds all the lines as an array of Line* lines.
  // This CollisionWorld owns the Line* lines.
  Line** lines;
  unsigned int numOfLines;

  // Record the total number of line-wall collisions.
  unsigned int numLineWallCollisions;

  // Record the total number of line-line intersections.
  unsigned int numLineLineCollisions;
};
typedef struct CollisionWorld CollisionWorld;

CollisionWorld* CollisionWorld_new(const unsigned int capacity);

void CollisionWorld_delete(CollisionWorld* collisionWorld);

// Return the total number of lines in the box.
unsigned int CollisionWorld_getNumOfLines(CollisionWorld* collisionWorld);

// Add a line into the box.  Must be under capacity.
// This CollisionWorld becomes owner of the Line* line.
void CollisionWorld_addLine(CollisionWorld* collisionWorld, Line *line);

// Get a line from box.
Line* CollisionWorld_getLine(CollisionWorld* collisionWorld,
                             const unsigned int index);

// Update lines' situation in the box.
void CollisionWorld_updateLines(CollisionWorld* collisionWorld);

// Update position of lines.
void CollisionWorld_updatePosition(CollisionWorld* collisionWorld);

// Handle line-wall collision.
void CollisionWorld_lineWallCollision(CollisionWorld* collisionWorld);

// Detect line-line intersection.
void CollisionWorld_detectIntersection(CollisionWorld* collisionWorld);

// Get total number of line-wall collisions.
unsigned int CollisionWorld_getNumLineWallCollisions(
    CollisionWorld* collisionWorld);

// Get total number of line-line intersections.
unsigned int CollisionWorld_getNumLineLineCollisions(
    CollisionWorld* collisionWorld);

// Update the two lines based on their intersection event.
// Precondition: compareLines(l1, l2) < 0 must be true.
void CollisionWorld_collisionSolver(CollisionWorld* collisionWorld, Line *l1,
                                    Line *l2,
                                    IntersectionType intersectionType);

#endif  // COLLISIONWORLD_H_
