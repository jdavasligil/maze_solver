#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_maze_solver(void) {
    Maze maze;
    char *maze_str =
        "\
###########S##\n\
#            #\n\
# ########## #\n\
# # #    #   #\n\
# # # ##   # #\n\
# # # ###### #\n\
#      #     #\n\
# ###### ### #\n\
# #       #  #\n\
####E#########\0";

    maze_from_str(&maze, maze_str); 

    Path test_path;
    Point curr = maze.start;

    solve_maze(&maze, &test_path);
 //   animate_maze(&maze, &test_path);
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

void test_heap(void) {
    printf("Making a new heap...\n");
    Heap *heap = new_heap();
    printf("Done!\n");

    printf("Testing insert...\n");
    assert(heap_insert(heap, (HeapNode){(Point){0,1}, 20}) == 1);
    assert(heap->size == 1);
    assert(heap_insert(heap, (HeapNode){(Point){0,1}, 40}) == 0);
    assert(heap->size == 1);
    assert(heap_insert(heap, (HeapNode){(Point){1,1}, 10}) == 1);
    assert(heap->size == 2);
    assert(heap_insert(heap, (HeapNode){(Point){1,2}, 30}) == 1);
    assert(heap_insert(heap, (HeapNode){(Point){2,2}, 1}) == 1);
    assert(heap->size == 4);
    printf("Success!\n");


    printf("Testing extract min...\n");
    assert(extract_min(heap).score == 1);
    assert(heap->size == 3);
    assert(extract_min(heap).score == 10);
    assert(extract_min(heap).score == 20);
    assert(extract_min(heap).score == 30);
    assert(extract_min(heap).score == INT_MAX);
    assert(heap->size == 0);
    printf("Success!\n");

    heap_insert(heap, (HeapNode){(Point){0,1}, 20});
    heap_insert(heap, (HeapNode){(Point){1,1}, 10});
    heap_insert(heap, (HeapNode){(Point){1,2}, 30});
    heap_insert(heap, (HeapNode){(Point){2,2}, 1});

    printf("Testing heap get...\n");
    assert(heap_get(heap, (Point){0,1}).score == 20);
    assert(heap_get(heap, (Point){1,1}).score == 10);
    assert(heap_get(heap, (Point){1,2}).score == 30);
    assert(heap_get(heap, (Point){2,2}).score == 1);
    extract_min(heap);
    assert(heap_get(heap, (Point){2,2}).score == INT_MAX);
    assert(heap_get(heap, (Point){0,1}).score == 20);
    assert(heap_get(heap, (Point){1,1}).score == 10);
    assert(heap_get(heap, (Point){1,2}).score == 30);
    extract_min(heap);
    assert(heap_get(heap, (Point){2,2}).score == INT_MAX);
    assert(heap_get(heap, (Point){0,1}).score == 20);
    assert(heap_get(heap, (Point){1,1}).score == INT_MAX);
    assert(heap_get(heap, (Point){1,2}).score == 30);
    extract_min(heap);
    assert(heap_get(heap, (Point){0,1}).score == INT_MAX);
    assert(heap_get(heap, (Point){1,2}).score == 30);
    extract_min(heap);
    assert(heap_get(heap, (Point){1,2}).score == INT_MAX);
    printf("Success!\n");

    printf("Testing destroy heap...\n");
    destroy_heap(heap);
    printf("Success!\n");

    printf("Test successful.\n");
}
