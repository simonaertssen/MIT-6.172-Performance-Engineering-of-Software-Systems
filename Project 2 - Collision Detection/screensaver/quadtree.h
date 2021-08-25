#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./line.h"
#include "./intersection_event_list.h"

#define QUAD 4
#define MAX_DEPTH 4
#define MAX_LINES 16

typedef struct Quadtree Quadtree;

struct Quadtree {
    // Pointer to the parent of this tree
    Quadtree* parent;

    // Quadtree quadrants as an array of children
    Quadtree* children;

    // Pointer to array of lines
    Line** lines;

    // vectors representing quadtree boundaries
    Vec p1; // North-west point
    Vec p2; // South-east point

    unsigned int num_lines; // Number of lines in the array
    unsigned int capacity; // Number of lines the array can hold

    // Depth of the quadtree
    unsigned int depth;
};

// Initialise a quadtree structure
Quadtree initialise_quadtree(Quadtree* parent, double x_min, double y_min, double x_max, double y_max, unsigned int depth);

// Create a new quadtree
Quadtree* make_quadtree(Quadtree* parent, double x_min, double y_min, double x_max, double y_max, unsigned int depth);
static inline void allocate_children(Quadtree* tree) __attribute__((always_inline));
static inline void make_space_for_more_lines(Quadtree* tree) __attribute__((always_inline));;
void destroy_quadtree(Quadtree* tree);

// Inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree);
unsigned int count_lines(Quadtree* tree);

// Test if line fits the current quadtree
static inline bool does_line_fit(Line* line, Quadtree* tree) __attribute__((always_inline));

// Once the tree is filled, compute the collisions
static inline void register_collision(Line* line1, Line* line2, IntersectionEventList* intersectionEventList) __attribute__((always_inline));
void detect_collisions(Quadtree* tree, IntersectionEventList* intersectionEventList);


#endif  // QUADTREE_H_
