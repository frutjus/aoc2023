#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct {
    int row, col;
} Point;

Point find_start(Grid grid) {
    iter(r, grid.height) iter(c, grid.width) if (grid.at[r][c] == 'S') return (Point) { r, c };
    printf("Could not find start\n");
    return (Point) { -1, -1 };
}

Point* find_connections(Grid grid, Point loc) {
    static Point ps[2];

    switch (grid.at[loc.row][loc.col]) {
        case '|':
            ps[0] = (Point) { loc.row - 1, loc.col };
            ps[1] = (Point) { loc.row + 1, loc.col };
            break;
        case '-':
            ps[0] = (Point) { loc.row, loc.col - 1 };
            ps[1] = (Point) { loc.row, loc.col + 1 };
            break;
        case 'L':
            ps[0] = (Point) { loc.row - 1, loc.col };
            ps[1] = (Point) { loc.row, loc.col + 1 };
            break;
        case 'J':
            ps[0] = (Point) { loc.row - 1, loc.col };
            ps[1] = (Point) { loc.row, loc.col - 1 };
            break;
        case '7':
            ps[0] = (Point) { loc.row, loc.col - 1 };
            ps[1] = (Point) { loc.row + 1, loc.col };
            break;
        case 'F':
            ps[0] = (Point) { loc.row, loc.col + 1 };
            ps[1] = (Point) { loc.row + 1, loc.col };
            break;
        case 'S':
            int i = 0;
            switch (grid.at[loc.row-1][loc.col]) {
                case '|':
                case '7':
                case 'F':
                    ps[i++] = (Point) { loc.row - 1, loc.col };
            }
            switch (grid.at[loc.row+1][loc.col]) {
                case '|':
                case 'L':
                case 'J':
                    ps[i++] = (Point) { loc.row + 1, loc.col };
            }
            switch (grid.at[loc.row][loc.col-1]) {
                case '-':
                case 'L':
                case 'F':
                    ps[i++] = (Point) { loc.row, loc.col - 1 };
            }
            switch (grid.at[loc.row][loc.col+1]) {
                case '-':
                case 'J':
                case '7':
                    ps[i++] = (Point) { loc.row, loc.col + 1 };
            }
            break;
        case '.':
        default:
            printf("Invalid point: %c\n", grid.at[loc.row][loc.col]);
            exit(1);
    }

    return ps;
}

int main() {
    Grid grid = gridFromString(read_file("inp/day10.txt"));

    Point start = find_start(grid);

    Point current = start, last = (Point) { -1, -1 };
    int distance = 0;

    do {
        Point *connections = find_connections(grid, current);

        Point next;

        if (connections[0].col == last.col && connections[0].row == last.row) {
            next = connections[1];
        } else {
            next = connections[0];
        }

        last = current;
        current = next;
        distance++;
    } while (current.col != start.col || current.row != start.row);

    int ans = distance / 2;

    printf("Answer = %d\n", ans);

    return 0;
}
