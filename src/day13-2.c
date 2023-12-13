#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

bool checkReflection(Grid grid, int line) {
    int differences = 0;
    for (int i = 0; line - i - 1 >= 0 && line + i < grid.height; i++) {
        iter(j, grid.width) {
            if (grid.at[line - i - 1][j] != grid.at[line + i][j]) {
                differences++;
                if (differences > 1) {
                    return false;
                }
            }
        }
    }
    if (differences != 1) {
        return false;
    }
    return true;
}

int find_reflection(Grid grid) {
    for (int line = 1; line < grid.height; line++) {
        if (checkReflection(grid, line)) {
            return line;
        }
    }
    return 0;
}

int main() {
    char *str = read_file("inp/day13.txt");

    ull ans = 0;

    int index = 0;
    while (*str != '\0') {
        char *grid_start = str;

        while (!peek(str, parse_newline) && *str != '\0') {
            skip_until((const char**)&str, parse_newline);
        }
        *(str++) = '\0';
        parse_newline((const char**)&str);

        Grid grid = gridFromString(grid_start);
        Grid gridt = transposeGrid(grid);

        printf("Grid %d | %dx%d\n", index, grid.height, grid.width);
        printGrid(grid);

        int horizref = find_reflection(grid);
        int vertref = find_reflection(gridt);

        if (vertref != 0) {
            printf("Vertical reflection at %d\n", vertref);

            ans += vertref;
        }
        if (horizref != 0) {
            printf("Horizontal reflection at %d\n", horizref);

            ans += horizref * 100;
        }

        printf("\n");
        index++;
        deleteGrid(grid);
        deleteGrid(gridt);
        //getchar();
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
