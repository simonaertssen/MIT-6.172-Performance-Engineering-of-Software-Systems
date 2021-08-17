#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./line.h"

#define QUAD 4
#define MAX_DEPTH 3
#define MAX_LINES 32

typedef struct Quadtree Quadtree;

struct Quadtree {
    // Pointer to the parent of this tree
    Quadtree* parent;

    // Quadtree quadrants as an array of children
    Quadtree** children;

    // Pointer to array of lines
    Line** lines;
    unsigned int num_lines; // Number of lines in the array
    unsigned int capacity; // Number of lines the array can hold

    // Depth of the quadtree
    unsigned int depth;

    // vectors representing quadtree boundaries
    Vec p1; // North-west point
    Vec p2; // South-east point
};

// Initialise a quadtree structure
Quadtree initialise_quadtree(Quadtree* parent, double x_lo, double y_lo, double x_hi, double y_hi, unsigned int depth);

// Create a new quadtree
Quadtree* make_quadtree(Quadtree* parent, double x_lo, double y_lo, double x_hi, double y_hi, unsigned int depth);
void destroy_quadtree(Quadtree* tree);

// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree);

#endif  // QUADTREE_H_
