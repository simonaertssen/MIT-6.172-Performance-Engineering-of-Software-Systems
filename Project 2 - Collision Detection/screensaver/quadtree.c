#include "./quadtree.h"
#include "./intersection_detection.h"
#include "./intersection_event_list.h"

#include "./line.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>


// Initialise a branch structure
Quadtree initialise_quadbranch(Quadtree* parent, double x, double y, float width, uint8_t depth) {
    Quadtree new_tree = {
        .parent = parent, .children = NULL,
        .lines = (Line**)malloc(sizeof(Line*) * MAX_LINES),
        .num_lines = 0, .capacity = MAX_LINES,
        .center = {.x = x, .y = y }, .width = width,
        .depth = depth
    };
    return new_tree;
}

Quadtree* make_quadbranch(Quadtree* parent, double x, double y, float width, uint8_t depth) {
    Quadtree* branch = (Quadtree*)malloc(sizeof(Quadtree));
    if (branch == NULL) return NULL;
    *branch = initialise_quadbranch(parent, x, y, width, depth);
    return branch;
}


// Create a new quadtree
Quadtree** make_quadtree(Quadtree* parent, double x, double y, float width, uint8_t depth, uint16_t* num_branches) {
    uint16_t capacity = 0;
    for (uint8_t i = 0; i < MAX_DEPTH; i++) capacity += pow(4, i);

    Quadtree** tree = (Quadtree**)malloc(sizeof(Quadtree*) * capacity);
    if (tree == NULL) return NULL;

    Quadtree* branch = make_quadbranch(parent, x, y, width, depth);
    tree[(*num_branches)++] = branch;
    return tree;
}


void allocate_children(Quadtree* branch, Quadtree** tree, uint16_t* num_branches) {
    branch->children = (Quadtree*)malloc(sizeof(Quadtree) * QUAD);
    // Compute half the distance between the bounds and either add or subtract that.
    double x, y, width = branch->width * 0.5;
    uint8_t diffx, diffy;

    Quadtree* child = NULL;
    for (uint8_t c = 0; c < QUAD; ++c) {
        diffx = c / 2 == 1 ? -1 : 1;
        x = branch->center.x - diffx * width;
        diffy = c % 2 == 1 ? -1 : 1;
        y = branch->center.y - diffy * width;

        child = make_quadbranch(branch, x, y, width, branch->depth + 1);
        branch->children[c] = *child;
        tree[(*num_branches)++] = child;
    }

    // Now reassign lines in the parent tree to the children. Use line position to find which child fits. 
    // If a line does not fit in the children, we need to put it back into the buffer of the parent.
    uint16_t num_lines = branch->num_lines;
    branch->num_lines = 0;

    // Now insert the lines back into the tree and its children.
    for (uint16_t i = 0; i < num_lines; i++) {
        insert_line(branch->lines[i], branch->children, tree, num_branches);
        branch->lines[i] = NULL;
    }
}


inline void make_space_for_more_lines(Quadtree* branch) {
    // If we reached deep into the tree but there is no more space for a line, 
    // allocate more memory by doubling the capacity.
    float times = 1.5;
    Line** tmp = (Line**)realloc(branch->lines, sizeof(Line*) * (uint16_t)(branch->capacity * times));
    if (tmp == NULL) free(tmp);
    else {
        branch->lines = tmp;
        branch->capacity *= times;
    }
}


void destroy_quadtree(Quadtree** tree, uint16_t* num_branches) {
    for (uint16_t i = 0; i < (*num_branches); i++) {
        free(tree[i]->lines);
        free(tree[i]);
    }
    // // Check if children are allocated:
    // if (tree->children != NULL) {
    //     // Destroy recursively
    //     for (uint8_t i = 0; i < QUAD; i++) {
    //         destroy_quadtree(tree->children + i);
    //     }
    //     // Free the tree
    //     free(tree->children);
    // }
    // free(tree->lines);
    // if (tree->depth == 0) free(tree);
}


inline bool does_line_fit(Line* restrict line, Quadtree* branch) {
    return
        (fmin(line->p1.x, line->p2.x) >= branch->center.x - branch->width) &&
        (fmax(line->p1.x, line->p2.x) < branch->center.x + branch->width) &&
        (fmin(line->p1.y, line->p2.y) >= branch->center.y - branch->width) &&
        (fmax(line->p1.y, line->p2.y) < branch->center.y + branch->width) &&
        // check line at end of time step of 0.5 (from collisionworld)
        (fmin(line->p1.x, line->p2.x) + line->velocity.x * 0.5 >= branch->center.x - branch->width) &&
        (fmax(line->p1.x, line->p2.x) + line->velocity.x * 0.5 < branch->center.x + branch->width) &&
        (fmin(line->p1.y, line->p2.y) + line->velocity.y * 0.5 >= branch->center.y - branch->width) &&
        (fmax(line->p1.y, line->p2.y) + line->velocity.y * 0.5 < branch->center.y + branch->width);
}


// inserts a line into a quadtree
void insert_line(Line* l, Quadtree* branch, Quadtree** tree, uint16_t* num_branches) {
    if (branch->num_lines < branch->capacity) {
        branch->lines[branch->num_lines++] = l;
        return;
    }

    // If we have no children, then save the line in level of the quadtree.
    if (branch->children == NULL) {

        // Do we need some more space for lines?
        if (branch->depth == MAX_DEPTH && branch->num_lines >= branch->capacity)
            make_space_for_more_lines(branch);

        // Add the line if there is enough capacity
        if (branch->num_lines < branch->capacity) {
            branch->lines[branch->num_lines++] = l;
            return;
        }

        // If we reach this point, then we have are not at the maximum depth and there is no space for lines,
        // so we need to allocate children of the branch and fill them with lines.
        allocate_children(branch, tree, num_branches);
    }

    // If we do have children (perhaps they were just allocated), then save the line l in the correct child.
    if (branch->children != NULL) {
        // Now add the line to the right child
        for (uint8_t j = 0; j < QUAD; j++) {
            if (does_line_fit(l, branch->children + j)) {
                insert_line(l, branch->children + j, tree, num_branches);
                return;
            }
            // The line does not fit any of the children. Do we need some more space for lines?
            if (branch->num_lines >= branch->capacity) make_space_for_more_lines(branch);
            // Add the line if there is enough capacity
            if (branch->num_lines < branch->capacity) {
                branch->lines[branch->num_lines++] = l;
                return;
            }
            // If we reached this point we have a problem...
            assert(0);
        }
    }
}


unsigned int count_lines(Quadtree** tree, uint16_t* num_branches) {
    // if (tree->children == NULL) {
    //     return tree->num_lines;
    // }

    // uint16_t num_lines = 0;
    // for (uint8_t j = 0; j < QUAD; j++) {
    //     num_lines += count_lines(tree->children + j);
    // }
    // return num_lines + tree->num_lines;

    unsigned int num_lines = 0;
    for (uint16_t i = 0; i < (*num_branches); i++) {
        num_lines += tree[i]->num_lines;
    }
    return num_lines;
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


void detect_collisions(Quadtree* branch, IntersectionEventList* restrict intersectionEventList) {
    assert(branch != NULL);

    Line* l1 = NULL;
    Line* l2 = NULL;

    uint16_t d, i, j;
    // Check all pairs in the current branch for collisions
    for (i = 0; i < branch->num_lines; i++) {
        l1 = branch->lines[i];
        for (j = i + 1; j < branch->num_lines; j++) {
            l2 = branch->lines[j];
            register_collision(l1, l2, intersectionEventList);
        }
    }

    // If there are children, then also check them
    if (branch->children != NULL) {
        for (uint8_t c = 0; c < QUAD; c++) {
            detect_collisions(branch->children + c, intersectionEventList);
        }
    }

    // Check all pairs in the current branch and its parents for collisions
    Quadtree* parent = branch;
    // Go to all possible parents.
    for (d = 0; d < branch->depth; d++) {
        // Register the parents parent
        parent = parent->parent;
        if (parent == NULL) return;

        // Mark all lines in this branch...
        for (i = 0; i < branch->num_lines; i++) {
            l1 = branch->lines[i];
            // ...versus all lines in the parent...
            for (j = 0; j < parent->num_lines; j++) {
                l2 = parent->lines[j];
                register_collision(l1, l2, intersectionEventList);
            }
        }
    }
}