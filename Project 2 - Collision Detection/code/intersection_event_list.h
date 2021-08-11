/**
 * Copyright (c) 2013 the Massachusetts Institute of Technology
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

#ifndef INTERSECTIONEVENTLIST_H_
#define INTERSECTIONEVENTLIST_H_

#include "./line.h"
#include "./intersection_detection.h"

struct IntersectionEventNode {
  // This IntersectionEventNode does not own these Line* lines.
  Line* l1;
  Line* l2;
  IntersectionType intersectionType;
  struct IntersectionEventNode* next;
};
typedef struct IntersectionEventNode IntersectionEventNode;

// Compares the nodes by l1's line ID, then l1's line ID.
// -1 <=> node1 ordered before node2
//  0 <=> node1 ordered the same as node2
//  1 <=> node1 ordered after node2
int IntersectionEventNode_compareData(IntersectionEventNode* node1,
                                      IntersectionEventNode* node2);

// Swaps the node1's and node2's data (l1, l2, intersectionType).
void IntersectionEventNode_swapData(IntersectionEventNode* node1,
                                    IntersectionEventNode* node2);

struct IntersectionEventList {
  IntersectionEventNode* head;
  IntersectionEventNode* tail;
};
typedef struct IntersectionEventList IntersectionEventList;

// Returns an empty list.
IntersectionEventList IntersectionEventList_make();

// Appends a new node to the list with the data (l1, l2, intersectionType).
// Precondition: compareLines(l1, l2) < 0 must be true.
void IntersectionEventList_appendNode(
    IntersectionEventList* intersectionEventList, Line* l1, Line* l2,
    IntersectionType intersectionType);

// Deletes all the nodes in the list.
void IntersectionEventList_deleteNodes(
    IntersectionEventList* intersectionEventList);

#endif  // INTERSECTIONEVENTLIST_H_
