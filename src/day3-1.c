#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

typedef struct {
    bool isPartNumber;
    int value;
} SchematicNumber;

bool isSymbol(char c) {
    return c != '.' && !isdigit(c);
}

bool hasSymbol(Grid grid, int r, int c) {
    bool has = false;
    
    for (int i = r - 1; i <= r + 1; i++) // map
    if (0 <= i && i < grid.height)       // filter
    for (int j = c - 1; j <= c + 1; j++) // map
    if (0 <= j && j < grid.width)        // filter
    if (i != r || j != c) {              // filter 
        has = has || isSymbol(grid.contents[i][j]);
    }

    return has;
}

SchematicNumber checkNumber(Grid grid, int r, int *c, SchematicNumber cur) {
    
    if (r >= grid.height || *c >= grid.width) {
        return cur;
    } else if (isdigit(grid.contents[r][*c])) {
        int v = charToDigit(grid.contents[r][*c]);
        bool symb = hasSymbol(grid, r, *c);

        (*c)++;
        return checkNumber(grid, r, c, (SchematicNumber) { .isPartNumber = cur.isPartNumber || symb, .value = cur.value * 10 + v });
    } else {
        (*c)++;
        return cur;
    }
}

int main() {
    Grid grid = gridFromString(read_file("inp/day3.txt"));

    int ans = 0;
    
    for (int r = 0; r < grid.height; r++) {
        for (int c = 0; c < grid.width;) {
            SchematicNumber num = checkNumber(grid, r, &c, (SchematicNumber) { .isPartNumber = false, .value = 0 });
            if (num.isPartNumber) {
                printf("Part number found at r=%d c=%d v=%d\n", r, c, num.value);
                ans += num.value;
            } else if (num.value != 0) {
                printf("Non-part number found at r=%d c=%d v=%d\n", r, c, num.value);
            }
        }
    }

    printf("Answer = %d\n", ans);

    return 0;
}