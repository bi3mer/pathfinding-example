#include "astar.h"
#include "da.h"

#include <math.h>
#include <stdlib.h>

int astar_compare(const void *node, const size_t priority)
{
    const AStarNode *n = (const AStarNode *)node;
    return (n->priority > priority) ? 1 : -1;
}

void astar_state_init(AStarState *state, Grid *grid, Point src, Point tgt,
                      Heuristic_Func heuristic)
{
    state->grid = grid;
    state->src = src;
    state->tgt = tgt;
    state->heuristic = heuristic;

    const size_t grid_size = (size_t)(grid->dimensions.x * grid->dimensions.y);

    state->queue = da_init(sizeof(AStarNode), 128);
    state->path = da_init(sizeof(Point), 128);
    state->came_from = malloc(sizeof(Point) * grid_size);
    state->cost_so_far = malloc(sizeof(float) * grid_size);

    for (size_t i = 0; i < grid_size; i++)
    {
        state->came_from[i].x = -1;
        state->came_from[i].y = -1;
        state->cost_so_far[i] = INFINITY;
    }

    size_t start_idx = grid_index(grid, src);
    state->cost_so_far[start_idx] = 0;
    state->came_from[start_idx] = src; // Parent of start is itself

    AStarNode *start = da_append((void **)&state->queue);
    start->priority = heuristic(src, tgt);
    start->point = src;

    state->found = false;
}

void astar_state_cleanup(AStarState *state)
{
    if (state)
    {
        da_cleanup(state->queue);
        da_cleanup(state->path);
        free(state->came_from);
        free(state->cost_so_far);
    }
}

bool astar_step(AStarState *state)
{

    return true;
}

bool astar(AStarState *state)
{
    while (astar_step(state))
    {
    }

    return state->found;
}
