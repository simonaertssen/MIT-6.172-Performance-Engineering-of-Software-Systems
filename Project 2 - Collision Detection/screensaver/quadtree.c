#include "./quadtree.h"
#include "./line.h"

#include <stdlib.h>

Quadtree initialise_quadtree(Quadtree* parent, double x_lo, double y_lo, double x_hi, double y_hi, unsigned int depth) {
    Quadtree new_tree = {
        .parent = parent,
        .NorthWest = NULL, .NorthEast = NULL, .SouthWest = NULL, .SouthEast = NULL,
        .lines = (Line**)malloc(sizeof(Line) * MAX_LINES),
        .numLines = 0, .depth = depth,
        .p1 = {.x = x_lo, .y = y_lo }, .p2 = {.x = x_hi, .y = y_hi }
    };
    return new_tree;
}

// Create a new quadtree
Quadtree* new_quadtree(Quadtree* parent, double x_lo, double y_lo, double x_hi, double y_hi, unsigned int depth) {
    Quadtree* tree = (Quadtree*)malloc(sizeof(Quadtree));
    *tree = initialise_quadtree(parent, x_lo, y_lo, x_hi, y_hi, depth);
    return tree;
}


// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree) {
    return;
};
