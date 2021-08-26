#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./line.h"
#include "./intersection_event_list.h"

#define QUAD 4
#define MAX_DEPTH 4
#define MIN_HEIGHT MAX_DEPTH
#define MAX_LINES 16

typedef struct Quadtree Quadtree;

struct Quadtree {
    // Pointer to the parent of this tree
    // Quadtree* parent;

    // Quadtree quadrants as an array of children
    Quadtree* children;

    // Pointer to array of lines
    Line** lines;

    // vectors representing quadtree boundaries
    Vec p1; // North-west point
    Vec p2; // South-east point

    unsigned int num_lines; // Number of lines in the array
    unsigned int capacity; // Number of lines the array can hold

    // Depth and height (= opposite of depth) of the quadtree
    unsigned int depth;
    unsigned int height;
};

// Initialise a quadtree structure
Quadtree initialise_quadtree(double x_min, double y_min, double x_max, double y_max, unsigned int depth, unsigned int height);
void print_quadtree(Quadtree* tree);

// Create a new quadtree
Quadtree* make_quadtree(double x_min, double y_min, double x_max, double y_max, unsigned int depth, unsigned int height);
static inline void allocate_children(Quadtree* restrict tree) __attribute__((always_inline));
static inline void make_space_for_more_lines(Quadtree* restrict tree) __attribute__((always_inline));;
void destroy_quadtree(Quadtree* restrict tree);

// Inserts a line into a quadtree
void insert_line(Line* restrict l, Quadtree* restrict tree);
unsigned int count_lines(Quadtree* restrict tree);

// Test if line fits the current quadtree
static inline bool does_line_fit(Line* restrict line, Quadtree* restrict tree) __attribute__((always_inline));

// Once the tree is filled, compute the collisions
static inline void register_collision(Line* restrict line1, Line* restrict line2, IntersectionEventList* intersectionEventList) __attribute__((always_inline));
void detect_collisions(Quadtree* restrict tree, IntersectionEventList* restrict intersectionEventList);


#endif  // QUADTREE_H_
