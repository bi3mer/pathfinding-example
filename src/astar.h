#ifndef _ASTAR_
#define _ASTAR_

#include "grid.h"

typedef struct
{
    float f_score;
    Point point;
} AStarNode;

typedef struct
{
    Grid *grid;
    Point src;
    Point tgt;
    AStarNode *open_set;
    Point *came_from;
    float *g_score;
    bool *closed_set;
    Point *path;
    bool found;
} AStarState;

extern AStarState *astar_state_init(Grid *grid, Point src, Point tgt);
extern bool astar_step(AStarState *state);
extern bool astar(AStarState *state);
extern void astar_state_cleanup(AStarState *state);

#endif
