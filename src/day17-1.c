#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct {
    int r, c;
} Position;

typedef struct {
    enum {
        up,
        down,
        left,
        right,
    } direction;
    int distance;
} Run;

int pathfind(const Grid *grid, Grid *visited, Position pos, Run run) {

    if ((pos.r == grid->height - 1 && pos.c == grid->width - 2) || (pos.r == grid->height - 2 && pos.c == grid->width - 1)) {
        return grid->at[grid->height - 1][grid->width - 1];
    }

    unsigned int leftCost = -1, rightCost = -1, upCost = -1, downCost = -1;
    Grid newvisited = copyGrid(*visited);
    newvisited.at[pos.r][pos.c] = 1;
    
    if (pos.c > 0 && !visited->at[pos.r][pos.c - 1]) {
        if (run.direction == left) {
            if (run.distance < 3) {
                leftCost = pathfind(grid, &newvisited, (Position) { pos.r, pos.c - 1 }, (Run) { left, run.distance + 1 });
            }
        } else {
            leftCost = pathfind(grid, &newvisited, (Position) { pos.r, pos.c - 1 }, (Run) { left, 1 });
        }
    }

    if (pos.c < grid->width - 1 && !visited->at[pos.r][pos.c + 1]) {
        if (run.direction == right) {
            if (run.distance < 3) {
                rightCost = pathfind(grid, &newvisited, (Position) { pos.r, pos.c + 1 }, (Run) { right, run.distance + 1 });
            }
        } else {
            rightCost = pathfind(grid, &newvisited, (Position) { pos.r, pos.c + 1 }, (Run) { right, 1 });
        }
    }

    if (pos.r > 0 && !visited->at[pos.r - 1][pos.c]) {
        if (run.direction == up) {
            if (run.distance < 3) {
                upCost = pathfind(grid, &newvisited, (Position) { pos.r - 1, pos.c }, (Run) { up, run.distance + 1 });
            }
        } else {
            upCost = pathfind(grid, &newvisited, (Position) { pos.r - 1, pos.c }, (Run) { up, 1 });
        }
    }

    if (pos.r < grid->height - 1 && !visited->at[pos.r + 1][pos.c]) {
        if (run.direction == down) {
            if (run.distance < 3) {
                downCost = pathfind(grid, &newvisited, (Position) { pos.r + 1, pos.c }, (Run) { down, run.distance + 1 });
            }
        } else {
            downCost = pathfind(grid, &newvisited, (Position) { pos.r + 1, pos.c }, (Run) { down, 1 });
        }
    }

    unsigned int cost = -1;
    cost = leftCost < cost ? leftCost : cost;
    cost = rightCost < cost ? rightCost : cost;
    cost = upCost < cost ? upCost : cost;
    cost = downCost < cost ? downCost : cost;

    deleteGrid(newvisited);

    return cost;
}

int main() {
    const char *str = read_file("inp/day17-sample.txt");

    Grid grid = gridFromString(str);
    Grid visited = copyGrid(grid);
    zeroGrid(visited);

    iter(r, grid.height) {
        iter(c, grid.width) {
            grid.at[r][c] = charToDigit(grid.at[r][c]);
        }
    }

    ull ans = pathfind(&grid, &visited, (Position) { 0, 0 }, (Run) { right, 0 });

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
