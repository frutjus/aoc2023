/*
You may need to run this to get it to work on Powershell:
$OutputEncoding = [Console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
*/

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
            if (loc.row > 0)
            switch (grid.at[loc.row-1][loc.col]) {
                case '|':
                case '7':
                case 'F':
                    ps[i++] = (Point) { loc.row - 1, loc.col };
            }
            if (loc.row < grid.height - 1)
            switch (grid.at[loc.row+1][loc.col]) {
                case '|':
                case 'L':
                case 'J':
                    ps[i++] = (Point) { loc.row + 1, loc.col };
            }
            if (loc.col > 0)
            switch (grid.at[loc.row][loc.col-1]) {
                case '-':
                case 'L':
                case 'F':
                    ps[i++] = (Point) { loc.row, loc.col - 1 };
            }
            if (loc.col < grid.width - 1)
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

char* show(char c) {
    switch (c) {
        case '|': return "│";
        case '-': return "─";
        case 'L': return "└";
        case 'J': return "┘";
        case '7': return "┐";
        case 'F': return "┌";
        case '.': return ".";
        case 'S': return "S";
        default:
            printf("Unrecognised: %c\n", c);
            exit(1);
    }
}

int main() {
    Grid grid = gridFromString(read_file("inp/day10.txt"));

    Point start = find_start(grid);

    Point current = start, last = (Point) { -1, -1 };
    int pipe[grid.height][grid.width];

    iter(r, grid.height) {
        iter(c, grid.width) {
            pipe[r][c] = 0;
        }
    }

    do {
        pipe[current.row][current.col] = 1;
        
        Point *connections = find_connections(grid, current);

        if (grid.at[current.row][current.col] == 'S') {
            if ((connections[0].col == current.col - 1 || connections[1].col == current.col - 1) && (connections[0].row == current.row - 1 || connections[1].row == current.row - 1)) {
                grid.at[current.row][current.col] = 'J';
            } else if ((connections[0].col == current.col - 1 || connections[1].col == current.col - 1) && (connections[0].row == current.row + 1 || connections[1].row == current.row + 1)) {
                grid.at[current.row][current.col] = '7';
            } else if ((connections[0].col == current.col + 1 || connections[1].col == current.col + 1) && (connections[0].row == current.row - 1 || connections[1].row == current.row - 1)) {
                grid.at[current.row][current.col] = 'L';
            } else if ((connections[0].col == current.col + 1 || connections[1].col == current.col + 1) && (connections[0].row == current.row + 1 || connections[1].row == current.row + 1)) {
                grid.at[current.row][current.col] = 'F';
            } else if (connections[0].col == current.col) {
                grid.at[current.row][current.col] = '|';
            } else if (connections[0].row == current.row) {
                grid.at[current.row][current.col] = '-';
            } else {
                printf("Unreachable\n");
                exit(1);
            }
        }

        Point next;

        if (connections[0].col == last.col && connections[0].row == last.row) {
            next = connections[1];
        } else {
            next = connections[0];
        }

        last = current;
        current = next;
    } while (current.col != start.col || current.row != start.row);

    iter(r, grid.height) {
        int parity = 0;
        iter(c, grid.width) {
            if (pipe[r][c] == 1) {
                switch (grid.at[r][c]) {
                    case '|':
                        parity += 2;
                        break;
                    case '-':
                        break;
                    case 'L':
                        parity += 1;
                        break;
                    case 'J':
                        parity -= 1;
                        break;
                    case '7':
                        parity += 1;
                        break;
                    case 'F':
                        parity -= 1;
                        break;
                }
            }
            if (pipe[r][c] == 0 && parity % 4 == 2) {
                pipe[r][c] = 2;
            }
        }
    }

    ull ans = 0;

    iter(r, grid.height) {
        iter(c, grid.width) {
            if (pipe[r][c] == 2) {
                ans++;
            }
            printf(pipe[r][c] == 1 ? "[32m%s[0m" : pipe[r][c] == 2 ? "[31m%s[0m" : "%s", show(grid.at[r][c]));
        }
        printf("\n");
    }

    printf("Answer = %llu\n", ans);

    return 0;
}
