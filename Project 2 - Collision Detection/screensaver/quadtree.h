#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./line.h"
#include "./intersection_event_list.h"

#include <stdint.h>

#define QUAD 4
#define MAX_DEPTH 8
#define MAX_LINES 16

typedef struct Quadtree Quadtree;

struct Quadtree {
    // Pointer to the parent of this tree
    Quadtree* parent;

    // Quadtree quadrants as an array of children
    Quadtree* children;

    // Pointer to array of lines
    Line** lines;

    // Vector representing quadtree center and its width
    Vec center;
    float width;

    uint16_t num_lines; // Number of lines in the array
    uint16_t capacity; // Number of lines the array can hold

    // Depth of the quadtree
    uint8_t depth;
};

// Create a new quadtree
Quadtree** make_quadtree(Quadtree* parent, double x, double y, float width, uint8_t depth, uint16_t* num_branches);

// Initialise a quadtree structure
Quadtree initialise_quadbranch(Quadtree* parent, double x, double y, float width, uint8_t depth);
Quadtree* make_quadbranch(Quadtree* parent, double x, double y, float width, uint8_t depth);

void allocate_children(Quadtree* branch, Quadtree** tree, uint16_t* num_branches);
void make_space_for_more_lines(Quadtree* branch);
void destroy_quadtree(Quadtree** tree, uint16_t* num_branches);

// Inserts a line into a quadtree
void insert_line(Line* l, Quadtree* branch, Quadtree** tree, uint16_t* num_branches);
unsigned int count_lines(Quadtree** tree, uint16_t* num_branches);

// Test if line fits the current quadtree
static inline bool does_line_fit(Line* restrict line, Quadtree* branch) __attribute__((always_inline));

// Once the tree is filled, compute the collisions
static inline void register_collision(Line* restrict line1, Line* restrict line2, IntersectionEventList* intersectionEventList) __attribute__((always_inline));
void detect_collisions(Quadtree* branch, IntersectionEventList* restrict intersectionEventList);


#endif  // QUADTREE_H_
