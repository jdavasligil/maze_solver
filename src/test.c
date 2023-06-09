#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_maze_solver(void) {
    Maze maze;
    char *maze_str = "######E##\n#       #\n#       #\n#       #\n##S######\0";

    maze_from_str(&maze, maze_str); 
    assert(maze.width == 9);
    assert(maze.height == 5);

    Path test_path;
    Point curr = maze.start;

    solve_maze(&maze, &test_path);
    animate_maze(&maze, &test_path);
}

void test_hash_function(void) {
    printf("Testing hash function...\n");

    assert(hash((Point){0,0}) == 0);
    assert(hash((Point){12,100}) == 10012);

    printf("Test successful.\n");
}

void test_hash_map(void) {
    printf("Making a new map...\n");
    Map *map = new_map();
    printf("Done!\n");

    printf("Testing insert...\n");
    assert(map_insert(map, (Point){2,4}, 0) == 1);
    assert(map_insert(map, (Point){7,5}, 1) == 1);
    assert(map_insert(map, (Point){300,300}, 1) == 0);
    printf("Success!\n");
    
    printf("Testing get...\n");
    assert(map_get(map, (Point){2,4}) == 0);
    assert(map_get(map, (Point){7,5}) == 1);
    printf("Success!\n");

    printf("Testing delete...\n");
    assert(map_delete(map, (Point){0,0}) == 0);
    assert(map_delete(map, (Point){300,300}) == 0);
    assert(map_delete(map, (Point){2,4}) == 1);
    assert(map_delete(map, (Point){7,5}) == 1);
    printf("Success!\n");

    printf("Testing destroy map...\n");
    destroy_map(map);
    printf("Success!\n");

    printf("Test successful.\n");
}
