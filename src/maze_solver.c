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

void fill_false(bool *arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = false;
    }
}

PointMap *new_pmap(void) {
    Point *buf = (Point *)malloc(MAX_MAP_SIZE * sizeof(Point));
    PointMap *pmap = (PointMap *)malloc(sizeof(PointMap));
    pmap->buffer = buf;

    for (int i = 0; i < MAX_MAP_SIZE; ++i) {
        pmap->buffer[i] = (Point){INT_MAX, INT_MAX};
    }

    return pmap;
}

void destroy_pmap(PointMap *pmap) {
    free(pmap->buffer);
    free(pmap);
}

bool pmap_insert(PointMap *pmap, Point key, Point val) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE) {
        pmap->buffer[hash_key] = val;

        return true;
    }

    return false;
}

bool pmap_delete(PointMap *pmap, Point key) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE && pmap->buffer[hash_key].x != INT_MAX) {
        pmap->buffer[hash_key] = (Point){INT_MAX, INT_MAX};

        return true;
    }

    return false;
}

Point pmap_get(PointMap *pmap, Point key) {
    int hash_key = hash(key);

    if (hash_key < MAX_MAP_SIZE) {
        return pmap->buffer[hash_key];
    }

    return (Point){INT_MAX, INT_MAX};
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
        return map->buffer[hash_key];
    }

    return -1;
}

// parent(i)      = (i - 1) / 2
// left_child(i)  =  2 * i + 1
// right_child(i) =  2 * i + 2
Heap *new_heap(void) { 
    Heap *heap = (Heap *)malloc(sizeof(Heap)); 
    heap->buffer = (HeapNode *)malloc(MAX_MAZE_SIZE * sizeof(HeapNode));
    heap->map = new_map();
    return heap;
}

void destroy_heap(Heap *heap) {
    destroy_map(heap->map);
    free(heap->buffer);
    free(heap);
}

int insert_helper(Heap *heap, int idx) {
    int parent = (idx - 1) / 2;

    if (parent < 0) {
        return idx;
    }

    if (heap->buffer[parent].score > heap->buffer[idx].score) {
        HeapNode tmp = heap->buffer[parent];
        heap->buffer[parent] = heap->buffer[idx];
        heap->buffer[idx] = tmp;

        map_insert(heap->map, tmp.point, idx);

        return insert_helper(heap, parent);
    }

    return idx;
}

// Heap maintains a unique set of points
bool heap_insert(Heap *heap, HeapNode node) {
    if (heap->size >= MAX_MAZE_SIZE) {
        return false;
    }

    if (map_get(heap->map, node.point) != -1) {
        heap->buffer[map_get(heap->map, node.point)].score = node.score;
        return true;
    }

    heap->buffer[heap->size] = node;

    map_insert(heap->map, node.point, insert_helper(heap, heap->size)); 

    heap->size += 1;

    return true;

}

// parent(i)      = (i - 1) / 2
// left_child(i)  =  2 * i + 1
// right_child(i) =  2 * i + 2
void minheapify(Heap *heap, int idx) {
    int left = idx * 2 + 1;
    int right = idx * 2 + 2;
    int min_idx = idx;

    if (left >= heap->size || left < 0) {
        left = -1;
    }

    if (right >= heap->size || right < 0) {
        right = -1;
    }

    if (left != -1 && heap->buffer[left].score < heap->buffer[idx].score) {
        min_idx = left;
    }

    if (right != -1 && heap->buffer[right].score < heap->buffer[idx].score) {
        min_idx = right;
    }

    if (min_idx != idx) {
        // Swap hashmap indices
        map_insert(heap->map, heap->buffer[min_idx].point, idx);
        map_insert(heap->map, heap->buffer[idx].point, min_idx);

        // Swap heap nodes
        HeapNode tmp = heap->buffer[min_idx];
        heap->buffer[min_idx] = heap->buffer[idx];
        heap->buffer[idx] = tmp;

        minheapify(heap, min_idx);
    }
}

HeapNode heap_get(Heap *heap, Point point) {
    int idx = map_get(heap->map, point);

    if (0 <= idx && idx < heap->size) {
        return heap->buffer[idx];
    }

    return (HeapNode){(Point){INT_MAX, INT_MAX}, INT_MAX};
}

HeapNode extract_min(Heap *heap) {
    HeapNode min_node;

    if (heap->size == 0) {
        return (HeapNode){(Point){INT_MAX, INT_MAX}, INT_MAX};
    }

    min_node = heap->buffer[0];

    map_delete(heap->map, min_node.point);

    heap->buffer[0] = heap->buffer[heap->size - 1];

    heap->size -= 1;

    minheapify(heap, 0);

    if (heap->size == 1) {
        map_insert(heap->map, heap->buffer[0].point, 0);
    }

    return min_node;
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

void reverse_path(Path *path) {
    int h = 0;
    int t = path->idx - 1;
    Point tmp;

    while (h < t) {
        tmp = path->buffer[t];
        path->buffer[t] = path->buffer[h];
        path->buffer[h] = tmp;

        ++h;
        --t;
    }
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
    printf("  ///////////////\n");
    printf("  //MAZE SOLVER//\n");
    printf("  ///////////////\n\n");
    printf("   ");
    for (int x = 0; x < maze->width; ++x) {
        printf("%c", (char)(x + 65));
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

void reconstruct_path(Path *path, PointMap *prev, Point p) {
    path_push(path, p);
    while (pmap_get(prev, p).x != INT_MAX) {
        p = pmap_get(prev, p);
        path_push(path, p);
    }
}

// TODO: Make heap for openset, Hashmap node -> heap idx
// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from n.
bool a_star(Maze *maze, Path *path, int (*h)()) {
    HeapNode curr;
    int tentative_g_score;
    Point neighbor;

    bool found_goal = false;
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known (f-score INF).
    // This is usually implemented as a min-heap or priority queue. 
    Heap *open_set = new_heap();
    heap_insert(open_set, (HeapNode){maze->start, h(maze->start, maze->end)});

    // For node n, cameFrom[n] is the node immediately preceding it on the
    // cheapest path from the start to n currently known.
    PointMap *came_from = new_pmap();

    // For node n, gScore[n] is the cost of the cheapest path from start to n
    // currently known.
    Map *g_score = new_map();
    map_insert(g_score, maze->start, 0);

    while (open_set->size > 0) {
        curr = extract_min(open_set);

        if (curr.point.x == maze->end.x && curr.point.y == maze->end.y) {
            reconstruct_path(path, came_from, curr.point);
            found_goal = true;
            break;
        }

        for (int d = 0; d < 4; ++d) {
            neighbor = (Point){curr.point.x + dir[d].x, curr.point.y + dir[d].y};
            if (
                    maze->buffer[neighbor.x + neighbor.y * maze->width].class == Wall ||
                    neighbor.x < 0 ||
                    neighbor.x >= maze->width ||
                    neighbor.y < 0 ||
                    neighbor.y >= maze->height 
               ) {
                continue;
            }

            tentative_g_score = map_get(g_score, curr.point) + 1;
             if (map_get(g_score, neighbor) == -1 || tentative_g_score < map_get(g_score, neighbor)) {
                 pmap_insert(came_from, neighbor, curr.point);
                 map_insert(g_score, neighbor, tentative_g_score);
                 heap_insert(open_set, (HeapNode){neighbor, tentative_g_score + h(neighbor, maze->end)});
             }
        }
    }

    destroy_heap(open_set);
    destroy_pmap(came_from);
    destroy_map(g_score);

    return found_goal;
}

void solve_maze(Maze *maze, Path *path) {
    a_star(maze, path, &manhattan_distance);
    reverse_path(path);
   //  bool seen[MAX_MAZE_SIZE];
   //  fill_false(seen, MAX_MAZE_SIZE);
   //  pathfinder(maze, maze->start, seen, path);
}
