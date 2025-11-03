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

    // const size_t src_idx = (size_t)(src.y * grid->dimensions.x + src.x);
    // state->came_from[src_idx] = src;
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
    if (state->found || da_length(state->queue) == 0)
        return false; // done processing

    Point p = state->queue[da_length(state->queue) - 1];
    da_pop_end(state->queue);

    if (point_equals(p, state->tgt))
    {
        state->found = true;

        while (!point_equals(p, state->src))
        {
            *(Point *)da_append((void **)&state->path) = p;
            p = state->came_from[grid_index(state->grid, p)];
        }

        *(Point *)da_append((void **)&state->path) = state->src;
        da_reverse(state->path);

        return false; // done processing
    }

    Point neighbors[4];
    const size_t num_neighbors = grid_neighbors(state->grid, p, neighbors);
    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const size_t id = grid_index(state->grid, neighbors[i]);
        if (state->came_from[id].x == -1)
        {
            state->came_from[grid_index(state->grid, neighbors[i])] = p;
            *(Point *)da_append((void **)&state->queue) = neighbors[i];
        }
    }

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
    if (state->found || da_length(state->queue) == 0)
        return false; // done processing

    Point p = state->queue[0];
    da_pop_start(state->queue);

    if (point_equals(p, state->tgt))
    {
        state->found = true;

        while (!point_equals(p, state->src))
        {
            *(Point *)da_append((void **)&state->path) = p;
            p = state->came_from[grid_index(state->grid, p)];
        }

        *(Point *)da_append((void **)&state->path) = state->src;
        da_reverse(state->path);

        return false; // done processing
    }

    Point neighbors[4];
    const size_t num_neighbors = grid_neighbors(state->grid, p, neighbors);
    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const size_t id = grid_index(state->grid, neighbors[i]);
        if (state->came_from[id].x == -1)
        {
            state->came_from[grid_index(state->grid, neighbors[i])] = p;
            *(Point *)da_append((void **)&state->queue) = neighbors[i];
        }
    }

    return true; // Still processing
}

bool bfs(PathfindingState *state)
{
    while (bfs_step(state))
    {
    }

    return state->found;
}
