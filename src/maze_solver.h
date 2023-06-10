#ifndef MAZE_SOLVER
#define MAZE_SOLVER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_MAZE_SIZE 1024
#define MAX_PATH_SIZE 64
#define MAX_MAP_SIZE 65536
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
    int height;
    int width;
} Maze;

typedef struct Map {
    int *buffer;
} Map;

typedef struct PointMap {
    Point *buffer;
} PointMap;

typedef struct HeapNode {
    Point point;
    int score;
} HeapNode;

typedef struct Heap {
    HeapNode *buffer;
    Map *map;
    int size;
} Heap;


int hash(Point P);

void fill_sentinel(Map *map);
void fill_false(bool *arr, int size);

PointMap *new_pmap(void);
void destroy_pmap(PointMap *pmap);

bool pmap_insert(PointMap *pmap, Point key, Point val);
bool pmap_delete(PointMap *pmap, Point key);
Point pmap_get(PointMap *pmap, Point key);

Map *new_map(void);
void destroy_map(Map *map);

bool map_insert(Map *map, Point key, int val);
bool map_delete(Map *map, Point key);
int map_get(Map *map, Point key);

Heap *new_heap(void);
void destroy_heap(Heap *heap);

int insert_helper(Heap *heap, int idx);
bool heap_insert(Heap *heap, HeapNode node);
void minheapify(Heap *heap, int idx);
HeapNode heap_get(Heap *heap, Point point);
HeapNode extract_min(Heap *heap);

bool path_push(Path *path, Point point);
bool path_pop(Path *path);

bool maze_from_str(Maze *maze, char *str);
void draw_scene(Maze *maze, Point *curr);
void animate_maze(Maze *maze, Path *path);
void clear_screen(void);

bool pathfinder(Maze *maze, Point curr, bool *seen, Path *path);
int manhattan_distance(Point A, Point B);
void reconstruct_path(Path *path, PointMap *prev, Point p);
bool a_star(Maze *maze, Path *path, int (*h)());
void solve_maze(Maze *maze, Path *path);


#endif // MAZE_SOLVER
