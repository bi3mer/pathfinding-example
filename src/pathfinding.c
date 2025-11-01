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
    state->explored = malloc(sizeof(Point) * grid_size);

    for (size_t i = 0; i < grid_size; i++)
    {
        state->explored[i].x = -1;
        state->explored[i].y = -1;
    }

    state->queue[0] = src;
    da_increment_length(state->queue);

    const size_t src_idx = (size_t)(src.y * grid->dimensions.x + src.x);
    state->explored[src_idx] = src;
}

void pathfinding_cleanup(PathfindingState *state)
{
    if (state)
    {
        da_cleanup(state->queue);
        state->queue = NULL;

        da_cleanup(state->path);
        state->path = NULL;

        free(state->explored);
        state->explored = NULL;

        state->grid = NULL;
    }
}

///////////////////////////// DFS /////////////////////////////
bool dfs_step(PathfindingState *state)
{
    if (state->found || da_length(state->queue) == 0)
        return false; // done processing

    const Point cur = state->queue[da_length(state->queue) - 1];
    da_pop_end(state->queue);

    if (point_equals(cur, state->tgt))
    {
        state->found = true;

        Point path_trace = state->tgt;
        while (!point_equals(state->src, path_trace))
        {
            da_ensure_capacity((void **)&state->path, 1);
            state->path[da_length(state->path)] = path_trace;
            da_increment_length(state->path);

            path_trace = state->explored[grid_index(state->grid, path_trace)];
        }

        da_ensure_capacity((void **)&state->path, 1);
        state->path[da_length(state->path)] = state->src;
        da_increment_length(state->path);

        da_reverse(state->path);

        return false; // done processing
    }

    Point neighbors[4];
    const size_t num_neighbors = grid_neighbors(state->grid, cur, neighbors);
    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const Point n = neighbors[i];
        const size_t id = grid_index(state->grid, n);

        if (state->explored[id].x == -1)
        {
            state->explored[id] = cur;

            da_ensure_capacity((void **)&state->queue, 1);
            state->queue[da_length(state->queue)] = n;
            da_increment_length(state->queue);
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

    const Point cur = state->queue[0];
    da_pop_start(state->queue);

    if (point_equals(cur, state->tgt))
    {
        state->found = true;

        Point path_trace = state->tgt;
        while (!point_equals(state->src, path_trace))
        {
            da_ensure_capacity((void **)&state->path, 1);
            state->path[da_length(state->path)] = path_trace;
            da_increment_length(state->path);

            path_trace = state->explored[grid_index(state->grid, path_trace)];
        }

        da_ensure_capacity((void **)&state->path, 1);
        state->path[da_length(state->path)] = state->src;
        da_increment_length(state->path);

        da_reverse(state->path);

        return false; // done processing
    }

    Point neighbors[4];
    const size_t num_neighbors = grid_neighbors(state->grid, cur, neighbors);
    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const Point n = neighbors[i];
        const size_t id = grid_index(state->grid, n);

        if (state->explored[id].x == -1)
        {
            state->explored[id] = cur;

            da_ensure_capacity((void **)&state->queue, 1);
            state->queue[da_length(state->queue)] = n;
            da_increment_length(state->queue);
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
