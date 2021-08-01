/**
 * Determine number of ways to place N queens on a NxN chess board so
 * that no queen can attack another (i.e., no two queens in any row,
 * column, or diagonal).
 *
 * TODO: Implement parallelizable queens.
 * TODO: Fix race conditions.
 */

#include <stdio.h>

static int count = 0;

void try(int row, int left, int right) {
    int poss, place;
    // If row bitvector is all 1s, a valid ordering of the queens exists.
    if (row == 0xFF) ++count;
    else {
        poss = ~(row | left | right) & 0xFF;
        while (poss != 0) {
            place = poss & -poss;
            try(row | place, (left | place) << 1, (right | place) >> 1);
            poss &= ~place;
        }
    }
}

int main(int argc, char* argv[]) {
    try(0, 0, 0);
    printf("There are %d solutions. \n", count);
    return 0;
}
