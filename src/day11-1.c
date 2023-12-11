#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct {
    int row, col;
} Point;

int main() {
    Grid grid = gridFromString(read_file("inp/day11.txt"));

    Array(Point) galaxies = Array_new(Point);
    iter(r, grid.height) {
        iter(c, grid.width) {
            if (grid.at[r][c] == '#') {
                Point p = { .row=r, .col=c };
                Array_push(galaxies, p);
            }
        }
    }

    printf("Galaxies: %llu\n", galaxies.size);

    Array(int) emptyRows = Array_new(int);
    iter(r, grid.height) {
        bool isEmpty = true;
        iter(c, grid.width) {
            if (grid.at[r][c] == '#') {
                isEmpty = false;
            }
        }
        if (isEmpty) {
            Array_push(emptyRows, r);
        }
    }

    printf("Empty rows: %llu\n", emptyRows.size);

    Array(int) emptyCols = Array_new(int);
    iter(c, grid.width) {
        bool isEmpty = true;
        iter(r, grid.height) {
            if (grid.at[r][c] == '#') {
                isEmpty = false;
            }
        }
        if (isEmpty) {
            Array_push(emptyCols, c);
        }
    }

    printf("Empty cols: %llu\n", emptyCols.size);

    for (int i = emptyRows.size; i --> 0;) {
        iter(j, galaxies.size) {
            if (galaxies.at[j].row > emptyRows.at[i]) {
                galaxies.at[j].row++;
            }
        }
    }

    for (int i = emptyCols.size; i --> 0;) {
        iter(j, galaxies.size) {
            if (galaxies.at[j].col > emptyCols.at[i]) {
                galaxies.at[j].col++;
            }
        }
    }

    ull ans = 0;

    iter(i, galaxies.size) {
        for (int j = i + 1; j < galaxies.size; j++) {
            ans += abs(galaxies.at[j].row - galaxies.at[i].row) + abs(galaxies.at[j].col - galaxies.at[i].col);
        }
    }

    printf("Answer = %llu\n", ans);

    return 0;
}
