#include "./quadtree.h"
#include "./intersection_detection.h"
#include "./intersection_event_list.h"

#include "./line.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>


// Initialise a quadtree structure
Quadtree initialise_quadtree(Quadtree* parent, double x, double y, float width, short depth) {
    Quadtree new_tree = {
        .parent = parent, .children = NULL,
        .lines = (Line**)malloc(sizeof(Line*) * MAX_LINES),
        .num_lines = 0, .capacity = MAX_LINES,
        .center = {.x = x, .y = y }, .width = width,
        .depth = depth
    };
    return new_tree;
}


// Create a new quadtree
Quadtree* make_quadtree(Quadtree* parent, double x, double y, float width, short depth) {
    Quadtree* tree = (Quadtree*)malloc(sizeof(Quadtree));
    if (tree == NULL) return NULL;
    *tree = initialise_quadtree(parent, x, y, width, depth);
    return tree;
}

void allocate_children(Quadtree* tree) {
    tree->children = (Quadtree*)malloc(sizeof(Quadtree) * QUAD);
    // Compute half the distance between the bounds and either add or subtract that.
    double x, y, width = tree->width * 0.5;
    short diffx, diffy;
    unsigned int i;
    for (i = 0; i < QUAD; ++i) {
        diffx = i / 2 == 1 ? -1 : 1;
        x = tree->center.x - diffx * width;
        diffy = i % 2 == 1 ? -1 : 1;
        y = tree->center.y - diffy * width;
        tree->children[i] = initialise_quadtree(tree, x, y, width, tree->depth + 1);
    }

    // Now reassign lines in the parent tree to the children. Use line position to find which child fits. 
    // If a line does not fit in the children, we need to put it back into the buffer of the parent.
    unsigned int num_lines = tree->num_lines;
    tree->num_lines = 0;

    // Now insert the lines back into the tree and its children.
    for (i = 0; i < num_lines; i++) {
        insert_line(tree->lines[i], tree->children);
        tree->lines[i] = NULL;
    }
}


inline void make_space_for_more_lines(Quadtree* tree) {
    // If we reached deep into the tree but there is no more space for a line, 
    // allocate more memory by doubling the capacity.
    float times = 1.5;
    Line** tmp = (Line**)realloc(tree->lines, sizeof(Line*) * (unsigned int)(tree->capacity * times));
    if (tmp == NULL) free(tmp);
    else {
        tree->lines = tmp;
        tree->capacity *= times;
    }
}


void destroy_quadtree(Quadtree* tree) {
    // Check if children are allocated:
    if (tree->children != NULL) {
        // Destroy recursively
        for (unsigned int i = 0; i < QUAD; i++) {
            destroy_quadtree(tree->children + i);
        }
        // Free the tree
        free(tree->children);
    }
    free(tree->lines);
    if (tree->depth == 0) free(tree);
}


inline bool does_line_fit(Line* restrict line, Quadtree* restrict tree) {
    return
        (fmin(line->p1.x, line->p2.x) >= tree->center.x - tree->width) &&
        (fmax(line->p1.x, line->p2.x) < tree->center.x + tree->width) &&
        (fmin(line->p1.y, line->p2.y) >= tree->center.y - tree->width) &&
        (fmax(line->p1.y, line->p2.y) < tree->center.y + tree->width) &&
        // check line at end of time step of 0.5 (from collisionworld)
        (fmin(line->p1.x, line->p2.x) + line->velocity.x * 0.5 >= tree->center.x - tree->width) &&
        (fmax(line->p1.x, line->p2.x) + line->velocity.x * 0.5 < tree->center.x + tree->width) &&
        (fmin(line->p1.y, line->p2.y) + line->velocity.y * 0.5 >= tree->center.y - tree->width) &&
        (fmax(line->p1.y, line->p2.y) + line->velocity.y * 0.5 < tree->center.y + tree->width);
}


// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* tree) {
    // If we have no children, then save the line in level of the quadtree.
    if (tree->children == NULL) {

        // Do we need some more space for lines?
        if (tree->depth == MAX_DEPTH && tree->num_lines >= tree->capacity)
            make_space_for_more_lines(tree);

        // Add the line if there is enough capacity
        if (tree->num_lines < tree->capacity) {
            tree->lines[tree->num_lines++] = l;
            return;
        }

        // If we reach this point, then we have are not at the maximum depth and there is no space for lines,
        // so we need to allocate children of the tree and fill them with lines.
        allocate_children(tree);
    }

    // If we do have children (perhaps they were just allocated), then save the line l in the correct child.
    if (tree->children != NULL) {
        // Now add the line to the right child
        for (unsigned int j = 0; j < QUAD; j++) {
            if (does_line_fit(l, tree->children + j)) {
                insert_line(l, tree->children + j);
                return;
            }
            // The line does not fit any of the children. Do we need some more space for lines?
            if (tree->num_lines >= tree->capacity) make_space_for_more_lines(tree);
            // Add the line if there is enough capacity
            if (tree->num_lines < tree->capacity) {
                tree->lines[tree->num_lines++] = l;
                return;
            }
            // If we reached this point we have a problem...
            assert(0);
        }
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
    return num_lines + tree->num_lines;
}


inline void register_collision(Line* restrict l1, Line* restrict l2, IntersectionEventList* intersectionEventList) {
    if (compareLines(l1, l2) >= 0) {
        Line* temp = l1;
        l1 = l2;
        l2 = temp;
    }

    IntersectionType intersectionType = intersect(l1, l2);
    if (intersectionType != NO_INTERSECTION) {
        IntersectionEventList_appendNode(intersectionEventList, l1, l2, intersectionType);
    }
}


void detect_collisions(Quadtree* restrict tree, IntersectionEventList* restrict intersectionEventList) {
    assert(tree != NULL);

    Line* l1 = NULL;
    Line* l2 = NULL;

    unsigned int d, c, i, j;
    // Check all pairs in the current tree for collisions
    for (i = 0; i < tree->num_lines; i++) {
        l1 = tree->lines[i];
        for (j = i + 1; j < tree->num_lines; j++) {
            l2 = tree->lines[j];
            register_collision(l1, l2, intersectionEventList);
        }
    }

    // Check all pairs in the current tree and its parents for collisions
    Quadtree* parent = tree;
    // Go to all possible parents.
    for (d = 0; d < tree->depth; d++) {
        // Register the parents parent
        parent = parent->parent;
        if (parent == NULL) return;

        // Mark all lines in this tree...
        for (i = 0; i < tree->num_lines; i++) {
            l1 = tree->lines[i];
            // ...versus all lines in the parent...
            for (j = 0; j < parent->num_lines; j++) {
                l2 = parent->lines[j];
                register_collision(l1, l2, intersectionEventList);
            }
        }
    }
    // If there are children, then also check them
    if (tree->children != NULL) {
        for (i = 0; i < QUAD; i++) {
            detect_collisions(tree->children + i, intersectionEventList);
        }
    }
}