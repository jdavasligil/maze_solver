#ifndef MAZE_SOLVER
#define MAZE_SOLVER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MAZE_SIZE 1024
#define MAX_PATH_SIZE 64
#define FRAME_TIME 500000

typedef enum {
    Wall,
    Floor,
    Start,
    End
} TileClass;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Path {
    Point buffer[MAX_PATH_SIZE];
    int idx;
} Path;

typedef struct Tile {
    TileClass class;
    Point position;
} Tile;

typedef struct Maze {
    Tile buffer[MAX_MAZE_SIZE];
    Point start;
    Point end;
    int h;
    int w;
} Maze;

bool path_push(Path *path, Point point);
bool path_pop(Path *path);

bool maze_from_str(Maze *maze, char *str);
void draw_scene(Maze *maze, Point *curr);
void animate_maze(Maze *maze, Path *path);
void clear_screen(void);

bool walk(Maze *maze, Point curr, bool *seen, Point *path);
void solve_maze(Maze *maze, Point *path);

#endif // MAZE_SOLVER
