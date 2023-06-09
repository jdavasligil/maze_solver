#include "maze_solver.h"

const Point dir[4] = {{1,0},{0,-1},{-1,0},{0,1}};

// Szudzik's hashing function for two positive integers.
int hash(Point P) {
    int a = P.x;
    int b = P.y;
    return (a >= b) * (a * a + a + b) + (a < b) * (a + b * b);
}

void fill_sentinel(Map *map) {
    for (int i = 0; i < MAX_MAP_SIZE; ++i) {
        map->buffer[i] = -1;
    }
}

Map *new_map(void) {
    int *buf = (int *)malloc(MAX_MAP_SIZE * sizeof(int));
    Map *map = (Map *)malloc(sizeof(Map));
    map->buffer = buf;
    fill_sentinel(map);

    return map;
}

void destroy_map(Map *map) {
    free(map->buffer);
    free(map);
}

bool map_insert(Map *map, Point key, int val) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE) {
        map->buffer[hash_key] = val;

        return true;
    }

    return false;
}

bool map_delete(Map *map, Point key) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE && map->buffer[hash_key] != -1) {
        map->buffer[hash_key] = -1;

        return true;
    }

    return false;
}

int map_get(Map *map, Point key) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE) {
        return map->buffer[hash(key)];
    }

    return -1;
}

Heap *new_heap(void) { 
    return (Heap *)malloc(sizeof(Heap));
}

void destroy_heap(Heap *heap) {
    free(heap);
}

void insert_helper(Heap *heap, int idx) {
    int parent = (idx - 1) / 2;

    if (parent < 0) {
        return;
    }

    if (heap->buffer[parent].x > heap->buffer[idx].x) {
        Point tmp = heap->buffer[parent];
        heap->buffer[parent] = heap->buffer[idx];
        heap->buffer[idx] = tmp;

        insert_helper(heap, parent);
    }
}

bool heap_insert(Heap *heap, Point point) {
    if (heap->size < MAX_MAZE_SIZE) {
        heap->buffer[heap->size] = point;

        insert_helper(heap, heap->size);

        heap->size += 1;
    }
}

void fill_false(bool *arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = false;
    }
}

bool path_push(Path *path, Point point) {
    if (path->idx >= MAX_PATH_SIZE) {
        return false;
    }

    path->buffer[path->idx].x = point.x;
    path->buffer[path->idx].y = point.y;
    path->idx += 1;

    return true;
}

bool path_pop(Path *path) {
    if (path->idx == 0) {
        return false;
    }

    path->idx -= 1;

    return true;
}

// '###\n'
bool maze_from_str(Maze *maze, char *str) {
    char *ptr = str;

    maze->width = 0;
    maze->height = 1;

    // Get width
    while (ptr && *ptr != '\n') {
        ++ptr;
        maze->width += 1;
    }

    // Get height
    while (ptr && *ptr != '\0') {
        ptr += 1 + maze->width;
        maze->height += 1;
    }

    ptr = str;

    if (maze->width * maze->height > MAX_MAZE_SIZE) {
        return false;
    }

    TileClass c = Floor;
    for (int y = 0; y < maze->height; ++y) {
        for (int x = 0; x < maze->width; ++x) {
            switch(*ptr) {
            case '#':
                c = Wall;
                break;
            case ' ':
                c = Floor;
                break;
            case 'S':
                maze->start = (Point) {x,y};
                c = Start;
                break;
            case 'E':
                maze->end = (Point) {x,y};
                c = End;
                break;
            }
            maze->buffer[x + y * maze->width] = (Tile) {
                c,
                {x,y}
            };
            ++ptr;
        }
        ++ptr;
    }

    return true;
}

void draw_scene(Maze *maze, Point *curr) {
    printf("///////////////\n");
    printf("//MAZE SOLVER//\n");
    printf("///////////////\n\n");
    printf("   ");
    for (int x = 0; x < maze->width; ++x) {
        printf("%d", x);
    }
    printf("\n");
    for (int y = 0; y < maze->height; ++y) {
        printf(" %d ", y);
        for (int x = 0; x < maze->width; ++x) {
            TileClass c = maze->buffer[x + y * maze->width].class;
            if (curr->x == x && curr->y == y) {
                printf("@");
            } else {
                switch(c) {
                case Wall:
                    printf("#");
                    break;
                case Floor:
                    printf(".");
                    break;
                case Start:
                    printf("S");
                    break;
                case End:
                    printf("E");
                    break;
                }
            }
        }
        printf("\n");
    }
}

void clear_screen(void) {
    system("clear");
}

void animate_maze(Maze *maze, Path *path) {
    for (int i = 0; i < path->idx; ++i) {
        clear_screen();
        draw_scene(maze, &path->buffer[i]);
        usleep(FRAME_TIME);
    }
}

bool pathfinder(
        Maze *maze,
        Point curr,
        bool *seen,
        Path *path
) {
    // Base Cases
    if (
               curr.x < 0
            || curr.x >= maze->width
            || curr.y < 0
            || curr.y >= maze->height
            || seen[curr.x + curr.y * maze->width]
            || maze->buffer[curr.x + curr.y * maze->width].class == Wall
            || path->idx >= MAX_PATH_SIZE
       ) {
        return false;
    }

    if (maze->buffer[curr.x + curr.y * maze->width].class == End) {
        path_push(path, curr);
        return true;
    }

    // Pre
    seen[curr.x + curr.y * maze->width] = true;
    path_push(path, curr);

    // Recursive Step
    for (int i = 0; i < 4; ++i) {
        if (pathfinder(
                    maze,
                    (Point){curr.x + dir[i].x, curr.y + dir[i].y},
                    seen,
                    path)
           ) {
            return true;
        }
    }

    path_pop(path);
    return false;
}

int manhattan_distance(Point A, Point B) {
    int dx = A.x - B.x;
    int dy = A.y - B.y;

    dx = (dx > 0) * dx + (dx <= 0) * (-dx);
    dy = (dy > 0) * dy + (dy <= 0) * (-dy);

    return dx + dy;
}

// TODO: Make heap for openset, Hashmap node -> heap idx
// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from n.
bool a_star(Maze *maze, Point curr, Path *path, int (*h)()) {
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue. 

    // For node n, cameFrom[n] is the node immediately preceding it on the
    // cheapest path from the start
    // to n currently known.
    return false;
}

void solve_maze(Maze *maze, Path *path) {
    bool seen[MAX_MAZE_SIZE];
    fill_false(seen, MAX_MAZE_SIZE);
    pathfinder(maze, maze->start, seen, path);
}
