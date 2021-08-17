#include "./quadtree.h"
#include "./line.h"

#include <stdlib.h>

// Initialise a quadtree structure
Quadtree initialise_quadtree(Quadtree* parent, double x_min, double y_min, double x_max, double y_max, unsigned int depth) {
    Quadtree new_tree = {
        .parent = parent, .children = NULL,
        .lines = (Line**)malloc(sizeof(Line*) * MAX_LINES),
        .num_lines = 0, .capacity = MAX_LINES, .depth = depth,
        .p1 = {.x = x_min, .y = y_min }, .p2 = {.x = x_max, .y = y_max }
    };
    return new_tree;
}

// Create a new quadtree
Quadtree* make_quadtree(Quadtree* parent, double x_min, double y_min, double x_max, double y_max, unsigned int depth) {
    Quadtree* tree = (Quadtree*)malloc(sizeof(Quadtree));
    if (tree == NULL) return NULL;
    *tree = initialise_quadtree(parent, x_min, y_min, x_max, y_max, depth);
    return tree;
}

void destroy_quadtree(Quadtree* tree) {
    // Check if children are allocated:
    if (tree->children != NULL) {
        for (unsigned int i = 0; i < QUAD; i++) {
            destroy_quadtree(tree->children + i);
        }
        free(tree->children);
    }
    // Free the lines
    free(tree->lines);
    if (tree->depth == 0) free(tree);
}


// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree) {
    // If we reached the maximum quadtree depth and have no more storage for lines,
    // then double the capacity by reallocating memory.
    if (tree->depth == MAX_DEPTH && tree->num_lines == MAX_LINES) {
        Line** tmp = (Line**)realloc(tree->lines, sizeof(Line*) * tree->capacity * 2);
        if (tmp == NULL) {
            free(tmp);
        }
        else {
            tree->lines = tmp;
            tree->capacity *= 2;
        }
    }

    // If the tree has enough storage, then use it and add the line
    if (tree->num_lines < MAX_LINES) {
        tree->lines[tree->num_lines++] = l;
    }
    // Else, we need to allocate the children of this quadtree
    else {
        tree->children = (Quadtree*)malloc(sizeof(Quadtree) * QUAD);
        // Make 2 loops of 2 to make four children, then we can compute the bounds more easily.
        for (unsigned int i = 0; i < 2; ++i) {
            for (unsigned int j = 0; j < 2; ++j) {
                tree->children[i * 2 + j] = initialise_quadtree(tree,
                    tree->p1.x + 0.5 * j * (tree->p2.x - tree->p1.x),
                    tree->p1.y + 0.5 * i * (tree->p2.y - tree->p1.y),
                    tree->p1.x + 0.5 * j * (tree->p2.x - tree->p1.x),
                    tree->p1.y + 0.5 * i * (tree->p2.y - tree->p1.y),
                    tree->depth + 1);
            }
        }
    }
};
