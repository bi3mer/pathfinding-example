#ifndef _ASTAR_
#define _ASTAR_

#include "grid.h"

typedef float (*Heuristic_Func)(const Point a, const Point b);

typedef struct
{
    Point point;
    float priority;
} AStarNode;

typedef struct
{
    Grid *grid;
    AStarNode *queue;
    Point *came_from;
    float *cost_so_far;
    Point *path;
    Heuristic_Func heuristic;
    bool found;
    Point src, tgt;
} AStarState;

extern void astar_state_init(AStarState *state, Grid *grid, Point src,
                             Point tgt, Heuristic_Func heuristic);
extern bool astar_step(AStarState *state);
extern bool astar(AStarState *state);
extern void astar_state_cleanup(AStarState *state);

#endif
