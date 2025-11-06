#include "pathfinding.h"
#include "da.h"
#include "grid.h"
#include "point.h"
#include <stdbool.h>

void pathfinding_init(PathfindingState *state, Grid *grid, Point src, Point tgt)
{
    state->grid = grid;

    state->src = src;
    state->tgt = tgt;
    state->found = false;

    state->queue = da_init(sizeof(Point), 128);
    state->path = da_init(sizeof(Point), 128);

    const size_t grid_size = (size_t)(grid->dimensions.x * grid->dimensions.y);
    state->came_from = malloc(sizeof(Point) * grid_size);

    for (size_t i = 0; i < grid_size; i++)
    {
        state->came_from[i].x = -1;
        state->came_from[i].y = -1;
    }

    state->queue[0] = src;
    da_increment_length(state->queue);
}

void pathfinding_cleanup(PathfindingState *state)
{
    if (state)
    {
        da_cleanup(state->queue);
        state->queue = NULL;

        da_cleanup(state->path);
        state->path = NULL;

        free(state->came_from);
        state->came_from = NULL;

        state->grid = NULL;
        state->found = false;
    }
}

///////////////////////////// DFS /////////////////////////////
bool dfs_step(PathfindingState *state)
{
    return true; // Still processing
}

bool dfs(PathfindingState *state)
{
    while (dfs_step(state))
    {
    }

    return state->found;
}

///////////////////////////// BFS /////////////////////////////
bool bfs_step(PathfindingState *state)
{
    return true; // Still processing
}

bool bfs(PathfindingState *state)
{
    while (bfs_step(state))
    {
    }

    return state->found;
}
