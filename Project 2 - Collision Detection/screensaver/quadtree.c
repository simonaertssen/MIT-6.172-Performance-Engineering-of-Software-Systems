#include "./quadtree.h"
#include "./intersection_detection.h"

#include "./line.h"

#include <stdlib.h>
#include <math.h>


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
    if (tree->children == NULL) {
        // Free the lines
        free(tree->lines);
    }
    else {
        // Destroy recursively
        for (unsigned int i = 0; i < QUAD; i++) {
            destroy_quadtree(tree->children + i);
        }
        // Free the tree
        free(tree->children);
    }
    if (tree->depth == 0) free(tree);
}


// Check if line can fit inside a given Quadtree's boundaries
inline bool does_line_fit(Line* line, Quadtree* tree) {
    return
        // check line at beginning of time step
        (fmin(line->p1.x, line->p2.x) >= tree->p1.x) &&
        (fmax(line->p1.x, line->p2.x) < tree->p2.x) &&
        (fmin(line->p1.y, line->p2.y) >= tree->p1.y) &&
        (fmax(line->p1.y, line->p2.y) < tree->p2.y) &&
        // check line at end of time step of 0.5 (from collisionworld)
        (fmin(line->p1.x, line->p2.x) + line->velocity.x * 0.5 >= tree->p1.x) &&
        (fmax(line->p1.x, line->p2.x) + line->velocity.x * 0.5 < tree->p2.x) &&
        (fmin(line->p1.y, line->p2.y) + line->velocity.y * 0.5 >= tree->p1.y) &&
        (fmax(line->p1.y, line->p2.y) + line->velocity.y * 0.5 < tree->p2.y);
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
        return;
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

        // Now reassign lines in the parent tree to the children
        for (unsigned int i = 0; i < tree->num_lines; i++) {
            // Use line position to find which child fits. Just try and fit for now,
            // an optimisation would be to compute the index of the quadrant.
            for (unsigned int j = 0; j < QUAD; j++) {
                if (does_line_fit(tree->lines[i], tree->children + j)) {
                    insert_line(tree->lines[i], tree->children + j);
                    tree->lines[i] = NULL;
                }
            }
        }
        // Empty the parent tree lines, as all lines are now distributed
        // free(tree->lines);
        tree->num_lines = 0;

        // Now add the line to the right child
        for (unsigned int j = 0; j < QUAD; j++) {
            if (does_line_fit(l, tree->children + j))
                insert_line(l, tree->children + j);
        }
    }
};


void detect_collisions(Quadtree* tree, unsigned int num_collisions) {
    if (tree == NULL) return;

    if (tree->children == NULL) {
    }
    else {
        for (unsigned int i = 0; i < QUAD; i++) {
            detect_collisions(tree->children + i, num_collisions);
        }
    }
}