#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

bool atceiling(Grid grid, int row, int col) {
    if (grid.at[row][col] == '#') {
        return false;
    }
    
    if (row == 0) {
        return true;
    }

    if (grid.at[row-1][col] == '#') {
        return true;
    }

    return false;
}

int countbeneath(Grid grid, int row, int col) {
    int count = 0;
    
    while (row < grid.height && grid.at[row][col] != '#') {
        if (grid.at[row][col] == 'O') {
            count++;
        }
        row++;
    }

    return count;
}

int triangle(int n) {
    return n * (n + 1) / 2;
}

int main() {
    char *str = read_file("inp/day14.txt");

    ull ans = 0;
    Grid grid = gridFromString(str);

    iter(row, grid.height) {
        iter(col, grid.width) {
            if (atceiling(grid, row, col)) {
                int count = countbeneath(grid, row, col);

                int load = triangle(grid.height - row) - triangle(grid.height - row - count);

                printf("Ceiling at (%d,%d) has %d rocks with %d load\n", row, col, count, load);

                ans += load;
            }
        }
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
