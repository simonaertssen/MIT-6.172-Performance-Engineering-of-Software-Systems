#include "./quadtree.h"
#include "./intersection_detection.h"
#include "./intersection_event_list.h"

#include "./line.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>


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
    if (tree->depth == MAX_DEPTH && tree->num_lines == tree->capacity) {
        printf("Needed reallocate\n");
        Line** tmp = (Line**)realloc(tree->lines, sizeof(Line*) * tree->capacity * 2);
        if (tmp == NULL) {
            free(tmp);
        }
        else {
            tree->lines = tmp;
            tree->capacity *= 2;
        }
    }

    if (tree->children == NULL) {
    }
    // If the tree has enough storage, then use it and add the line
    if (tree->num_lines < tree->capacity && tree->children == NULL) {
        tree->lines[tree->num_lines++] = l;
        return;
    }
    // Else, we need to allocate the children of this quadtree
    else {
        assert(tree->children == NULL);
        printf("Allocated children at depth %u\n", tree->depth);
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
        tree->capacity = 0;
    }
}

unsigned int count_lines(Quadtree* tree) {
    if (tree->children == NULL) {
        return tree->num_lines;
    }

    unsigned int num_lines = 0;
    for (unsigned int j = 0; j < QUAD; j++) {
        num_lines += count_lines(tree->children + j);
    }
    return num_lines;
}


void detect_collisions(Quadtree* tree, IntersectionEventList intersectionEventList, unsigned int* num_collisions) {
    // If there is no tree, we have an issue
    if (tree == NULL) return;
    printf("%u, \n", count_lines(tree));

    // If there are no children, check this tree
    if (tree->children == NULL) {
        for (unsigned int i = 0; i < tree->num_lines; i++) {
            Line* l1 = tree->lines[i];
            for (unsigned int j = i + 1; j < tree->num_lines; j++) {
                Line* l2 = tree->lines[j];

                if (compareLines(l1, l2) >= 0) {
                    Line* temp = l1;
                    l1 = l2;
                    l2 = temp;
                }

                IntersectionType intersectionType = intersect(l1, l2);
                if (intersectionType != NO_INTERSECTION) {
                    IntersectionEventList_appendNode(&intersectionEventList, l1, l2,
                        intersectionType);
                    (*num_collisions)++;
                }
            }
        }
    }
    else {
        for (unsigned int i = 0; i < QUAD; i++) {
            detect_collisions(tree->children + i, intersectionEventList, num_collisions);
        }
    }
}