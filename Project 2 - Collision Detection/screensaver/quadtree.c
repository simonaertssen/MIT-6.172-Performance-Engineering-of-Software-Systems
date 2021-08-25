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

void allocate_children(Quadtree* tree) {
    tree->children = (Quadtree*)malloc(sizeof(Quadtree) * QUAD);
    // Compute half the distance between the bounds and either add or subtract that.
    double col, row, minx, maxx, miny, maxy, diffx, diffy;
    for (unsigned int i = 0; i < QUAD; ++i) {
        col = (i / 2);
        row = i % 2;

        diffx = 0.5 * (tree->p2.x - tree->p1.x);
        minx = tree->p1.x + col * diffx;
        maxx = tree->p2.x - (double)(!col) * diffx;

        diffy = 0.5 * (tree->p2.y - tree->p1.y);
        miny = tree->p1.y + row * diffy;
        maxy = tree->p2.y - (double)(!row) * diffy;

        // printf("Grid[%d,%d]: x = [%f, %f], y = [%f, %f]\n", (i / 2), (i % 2), minx, maxx, miny, maxy);
        tree->children[i] = initialise_quadtree(tree, minx, miny, maxx, maxy, tree->depth + 1);
    }

    // Now reassign lines in the parent tree to the children. Use line position to find which child fits. 
    // If a line does not fit in the children, we need to put it back into the buffer of the parent.
    unsigned int num_lines = tree->num_lines;
    Line** all_lines = (Line**)malloc(sizeof(Line*) * num_lines);
    for (unsigned int i = 0; i < num_lines; i++) {
        all_lines[i] = tree->lines[i];
        tree->lines[i] = NULL;
    }
    tree->num_lines = 0;

    // Now insert the lines back into the tree and its children.
    for (unsigned int i = 0; i < num_lines; i++) {
        insert_line(all_lines[i], tree->children);
    }
    // Empty the parent tree lines, as all lines are now distributed
    free(all_lines);
}


void make_space_for_more_lines(Quadtree* tree) {
    // If we reached deep into the tree but there is no more space for a line, 
    // allocate more memory by doubling the capacity.
    int times = 2;
    Line** tmp = (Line**)realloc(tree->lines, sizeof(Line*) * tree->capacity * times);
    if (tmp == NULL) free(tmp);
    else {
        tree->lines = tmp;
        tree->capacity *= times;
    }
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
            printf("Line id = %u did not fit\n", l->id);
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


void register_collision(Line* l1, Line* l2, IntersectionEventList intersectionEventList, unsigned int* num_collisions) {
    // printf("Comparing l=%u with l=%d\n", l1->id, l2->id);
    if (compareLines(l1, l2) >= 0) {
        // printf("Switching l=%u with l=%d\n", l1->id, l2->id);
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


void detect_collisions(Quadtree* tree, IntersectionEventList intersectionEventList, unsigned int* num_collisions) {
    // If there are no children, check this tree
    if (tree->children == NULL) {
        Line* l1 = NULL;
        Line* l2 = NULL;
        // Check all pairs in the current tree for collisions
        for (unsigned int i = 0; i < tree->num_lines; i++) {
            l1 = tree->lines[i];
            for (unsigned int j = i + 1; j < tree->num_lines; j++) {
                l2 = tree->lines[j];
                register_collision(l1, l2, intersectionEventList, num_collisions);
            }
        }
        // Check all pairs in the current tree and its parents for collisions
        Quadtree* parent = tree->parent;
        // Go to all possible parents.
        for (unsigned int d = 0; d < tree->depth; d++) {
            // Mark all lines in this tree...
            for (unsigned int i = 0; i < tree->num_lines; i++) {
                l1 = tree->lines[i];
                // ...versus all lines in the parent...
                for (unsigned int j = 0; j < parent->num_lines; j++) {
                    l2 = parent->lines[j];
                    register_collision(l1, l2, intersectionEventList, num_collisions);
                }
            }

            // Register the parents parent
            parent = parent->parent;
            if (parent == NULL) return;
        }
    }
    else {
        for (unsigned int i = 0; i < QUAD; i++) {
            detect_collisions(tree->children + i, intersectionEventList, num_collisions);
        }
    }
}