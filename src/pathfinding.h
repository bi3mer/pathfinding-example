#ifndef _PATHFINDING_
#define _PATHFINDING_

#include "grid.h"
#include "point.h"

typedef struct
{
    Point *queue;
    Point *path;
    Point *explored;
    float *costs;
    Grid *grid;
    Point src, tgt;
    bool found;
} PathfindingState;

extern void pathfinding_init(PathfindingState *state, Grid *grid, Point src,
                             Point tgt);
extern void pathfinding_cleanup(PathfindingState *state);

extern bool dfs_step(PathfindingState *state);
extern bool bfs_step(PathfindingState *state);

extern bool dfs(PathfindingState *state);
extern bool bfs(PathfindingState *state);

#endif
