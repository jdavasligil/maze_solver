#include "maze_solver.h"

const Point dir[4] = {{1,0},{0,-1},{-1,0},{0,1}};

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

    maze->w = 0;
    maze->h = 1;

    // Get width
    while (ptr && *ptr != '\n') {
        ++ptr;
        maze->w += 1;
    }

    // Get height
    while (ptr && *ptr != '\0') {
        ptr += 1 + maze->w;
        maze->h += 1;
    }

    ptr = str;

    if (maze->w * maze->h > MAX_MAZE_SIZE) {
        return false;
    }

    TileClass c = Floor;
    for (int y = 0; y < maze->h; ++y) {
        for (int x = 0; x < maze->w; ++x) {
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
            maze->buffer[x + y * maze->w] = (Tile) {
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
    for (int x = 0; x < maze->w; ++x) {
        printf("%d", x);
    }
    printf("\n");
    for (int y = 0; y < maze->h; ++y) {
        printf(" %d ", y);
        for (int x = 0; x < maze->w; ++x) {
            TileClass c = maze->buffer[x + y * maze->w].class;
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

bool walk(
        Maze *maze,
        Point curr,
        bool *seen,
        Point *path
) {
    if (seen[curr.x + curr.y * maze->w]) {
        return false;
    }
    return false;
}
void solve_maze(Maze *maze, Point *path) {
}