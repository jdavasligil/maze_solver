#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_maze_solver(void) {
    Maze maze;
    char *maze_str = "######E##\n#       #\n#       #\n#       #\n##S######\0";

    maze_from_str(&maze, maze_str); 
    assert(maze.w == 9);
    assert(maze.h == 5);

    Path test_path;
    Point curr = maze.start;

//    path_push(&test_path, curr);
//    assert(test_path.idx == 1);
//
//    curr.y -= 1;
//    path_push(&test_path, curr);
//
//    curr.y -= 1;
//    path_push(&test_path, curr);
//
//    curr.x += 1;
//    path_push(&test_path, curr);
//
//    curr.x += 1;
//    path_push(&test_path, curr);
//
//    curr.x += 1;
//    path_push(&test_path, curr);
//
//    curr.x += 1;
//    path_push(&test_path, curr);
//
//    curr.y -= 1;
//    path_push(&test_path, curr);
//
//    curr.y -= 1;
//    path_push(&test_path, curr);

    solve_maze(&maze, &test_path);
    animate_maze(&maze, &test_path);
}
