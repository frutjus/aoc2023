#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef enum {
    right = 1,
    left = 2,
    up = 4,
    down = 8,
} Direction;

typedef struct {
    int r, c;
} Position;

typedef struct {
    Position position;
    Direction direction;
} Beam;

Grid grid, visited;

Position advance(Position pos, Direction dir) {
    switch (dir) {
        case right:
            return (Position) { .r = pos.r, .c = pos.c + 1 };
        case left:
            return (Position) { .r = pos.r, .c = pos.c - 1 };
        case up:
            return (Position) { .r = pos.r - 1, .c = pos.c };
        case down:
            return (Position) { .r = pos.r + 1, .c = pos.c };
        default:
            printf("Unrecognised direction: [%d]\n", dir);
            exit(1);
    }
}

void trace_beam(Beam beam) {
    if (beam.position.r < 0 || beam.position.r >= grid.height || beam.position.c < 0 || beam.position.c >= grid.height) {
        return;
    }

    if (visited.at[beam.position.r][beam.position.c] & beam.direction) {
        return;
    }

    visited.at[beam.position.r][beam.position.c] |= beam.direction;

    Beam nextbeam;

    switch (grid.at[beam.position.r][beam.position.c]) {
        case '.':
            nextbeam.direction = beam.direction;
            nextbeam.position = advance(beam.position, nextbeam.direction);
            trace_beam(nextbeam);
            return;
        case '/':
            switch (beam.direction) {
                case right:
                    nextbeam.direction = up;
                    break;
                case left:
                    nextbeam.direction = down;
                    break;
                case up:
                    nextbeam.direction = right;
                    break;
                case down:
                    nextbeam.direction = left;
                    break;
            }
            nextbeam.position = advance(beam.position, nextbeam.direction);
            trace_beam(nextbeam);
            return;
        case '\\':
            switch (beam.direction) {
                case right:
                    nextbeam.direction = down;
                    break;
                case left:
                    nextbeam.direction = up;
                    break;
                case up:
                    nextbeam.direction = left;
                    break;
                case down:
                    nextbeam.direction = right;
                    break;
            }
            nextbeam.position = advance(beam.position, nextbeam.direction);
            trace_beam(nextbeam);
            return;
        case '-':
            switch (beam.direction) {
                case right:
                case left:
                    nextbeam.direction = beam.direction;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
                case up:
                    nextbeam.direction = left;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    nextbeam.direction = right;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
                case down:
                    nextbeam.direction = left;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    nextbeam.direction = right;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
            }

        case '|':
            switch (beam.direction) {
                case right:
                    nextbeam.direction = up;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    nextbeam.direction = down;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
                case left:
                    nextbeam.direction = up;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    nextbeam.direction = down;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
                case up:
                case down:
                    nextbeam.direction = beam.direction;
                    nextbeam.position = advance(beam.position, nextbeam.direction);
                    trace_beam(nextbeam);
                    return;
            }
        default:
            printf("Unrecognised piece: [%c/%d]\n", grid.at[beam.position.r][beam.position.c], grid.at[beam.position.r][beam.position.c]);
            exit(1);
    }
}

int main() {
    const char *str = read_file("inp/day16.txt");

    grid = gridFromString(str);
    visited = copyGrid(grid);
    iter(r, grid.height) {
        memset(visited.at[r], 0, grid.width);
    }

    trace_beam((Beam) { .direction = right, .position = { .r = 0, .c = 0 } });

    ull ans = 0;

    iter(r, grid.height) {
        iter(c, grid.width) {
            if (visited.at[r][c]) {
                printf("[33m%c[0m", grid.at[r][c]);
                ans++;
            } else {
                printf("%c", grid.at[r][c]);
            }
        }
        printf("\n");
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
