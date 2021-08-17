#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./line.h"

#define MAX_DEPTH 3
#define MAX_ITEMS 32

typedef struct Quadtree Quadtree;

struct Quadtree {
    // Pointer to the parent of this tree
    Quadtree* parent;

    // Quadtree quadrants
    Quadtree* NorthWest;
    Quadtree* NorthEast;
    Quadtree* SouthWest;
    Quadtree* SouthEast;

    // Pointer to array of lines
    Line** lines;
    unsigned int numOfLines; // Number of lines in the array
    unsigned int capacity;   // Number of lines the array can hold

    // Depth of the quadtree
    unsigned int depth;

    // vectors representing quadtree boundaries
    Vec p1; // North-west point
    Vec p2; // South-east point
};

// Create a new quadtree
Quadtree new_quadtree(Quadtree* parent, double x_lo, double y_lo, double x_hi, double y_hi, unsigned int depth);

// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree);

#endif  // QUADTREE_H_
