#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

int getNumber(Grid grid, int r, int c) {
    if (r < 0 || r >= grid.height || c < 0 || c >= grid.width || !isdigit(grid.contents[r][c])) {
        return -1;
    }

    // find start of number
    while (c - 1 >= 0 && isdigit(grid.contents[r][c-1])) {
        c--;
    }

    int start = c;

    int num = charToDigit(grid.contents[r][c]);

    while (c + 1 < grid.width && isdigit(grid.contents[r][c+1])) {
        c++;
        num = num * 10 + charToDigit(grid.contents[r][c]);
    }

    return num;
}

int main() {
    Grid grid = gridFromString(read_file("inp/day3.txt"));

    int ans = 0;
    
    for (int r = 0; r < grid.height; r++) {
        for (int c = 0; c < grid.width; c++) {
            if (grid.contents[r][c] == '*') {
                int numCount = 0;
                int product = 1;
                // Check the possible positions of numbers manually...
                // Check neighbours above the current position
                if (r - 1 >= 0) {
                    // If there is a digit directly above, there can be only one number above
                    if (isdigit(grid.contents[r-1][c])) {
                        product *= getNumber(grid, r-1, c);
                        numCount++;
                    // Otherwise there might be two (top-left and/or top-right)
                    } else {
                        if (c - 1 >= 0 && isdigit(grid.contents[r-1][c-1])) {
                            product *= getNumber(grid, r-1, c-1);
                            numCount++;
                        }
                        if (c + 1 < grid.width && isdigit(grid.contents[r-1][c+1])) {
                            product *= getNumber(grid, r-1, c+1);
                            numCount++;
                        }
                    }
                }
                // Same logic for below
                if (r + 1 < grid.height) {
                    if (isdigit(grid.contents[r+1][c])) {
                        product *= getNumber(grid, r+1, c);
                        numCount++;
                    } else {
                        if (c - 1 >= 0 && isdigit(grid.contents[r+1][c-1])) {
                            product *= getNumber(grid, r+1, c-1);
                            numCount++;
                        }
                        if (c + 1 < grid.width && isdigit(grid.contents[r+1][c+1])) {
                            product *= getNumber(grid, r+1, c+1);
                            numCount++;
                        }
                    }
                }
                //Check to the left
                if (c - 1 >= 0 && isdigit(grid.contents[r][c-1])) {
                    product *= getNumber(grid, r, c-1);
                    numCount++;
                }
                //Check to the right
                if (c + 1 < grid.width && isdigit(grid.contents[r][c+1])) {
                    product *= getNumber(grid, r, c+1);
                    numCount++;
                }

                if (numCount == 2) {
                    ans += product;
                }
            }
        }
    }

    printf("Answer = %d\n", ans);

    return 0;
}