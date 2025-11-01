#include "pathfinding.h"
#include "da.h"
#include "grid.h"
#include "terrain.h"

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
    state->costs = malloc(sizeof(float) * grid_size);

    for (size_t i = 0; i < grid_size; i++)
    {
        state->explored[i].x = -1;
        state->explored[i].y = -1;
        state->costs[i] = terrain_to_cost(grid->grid[i]);
    }

    state->queue[0] = src;
    da_increment_length(state->queue);

    const size_t src_idx = (size_t)(src.y * grid->dimensions.x + src.x);
    state->explored[src_idx] = src;
    state->costs[src_idx] = 0.0f;
}

void pathfinding_cleanup(PathfindingState *state)
{
    if (state)
    {
        da_free(state->queue);
        state->queue = NULL;

        da_free(state->path);
        state->path = NULL;

        free(state->explored);
        state->explored = NULL;

        free(state->costs);
        state->costs = NULL;

        state->grid = NULL;
    }
}

///////////////////////////// DFS /////////////////////////////
bool dfs_step(PathfindingState *state)
{
    if (state->found || da_length(state->queue) == 0)
    {
        return false; // Done processing
    }

    size_t id;

    size_t queue_length = da_length(state->queue) - 1;
    Point cur = state->queue[queue_length];
    da_pop_end(state->queue);

    // Check if we found the target
    if (cur.x == state->tgt.x && cur.y == state->tgt.y)
    {
        state->found = true;

        // Reconstruct the path
        Point trace = cur;
        while (trace.x != state->src.x || trace.y != state->src.y)
        {
            da_ensure_capacity((void *)&state->path, 1);
            state->path[da_length(state->path)] = trace;
            da_increment_length(state->path);

            id = (size_t)(trace.y * state->grid->dimensions.x + trace.x);
            trace = state->explored[id];
        }

        // Add source to the path
        da_ensure_capacity((void *)&state->path, 1);
        state->path[da_length(state->path)] = state->src;
        da_increment_length(state->path);

        // Reverse the path to go from source to target
        size_t path_len = da_length(state->path);
        for (size_t i = 0; i < path_len / 2; i++)
        {
            Point temp = state->path[i];
            state->path[i] = state->path[path_len - 1 - i];
            state->path[path_len - 1 - i] = temp;
        }

        return false; // Done - found path
    }

    Point neighbors[4];
    size_t num_neighbors = grid_neighbors(state->grid, cur, neighbors);
    for (size_t i = 0; i < num_neighbors; i++)
    {
        const Point next = neighbors[i];
        id = (size_t)(next.y * state->grid->dimensions.x + next.x);

        if (state->explored[id].x == -1)
        {
            state->explored[id] = cur;

            da_ensure_capacity((void *)&state->queue, 1);
            size_t l = da_length(state->queue);
            state->queue[l] = next;
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
    {
        return false; // Done processing
    }

    size_t id;

    Point cur = state->queue[0];
    da_pop_start(state->queue);

    // Check if we found the target
    if (cur.x == state->tgt.x && cur.y == state->tgt.y)
    {
        state->found = true;

        // Reconstruct the path
        Point trace = cur;
        while (trace.x != state->src.x || trace.y != state->src.y)
        {
            da_ensure_capacity((void *)&state->path, 1);
            state->path[da_length(state->path)] = trace;
            da_increment_length(state->path);

            id = (size_t)(trace.y * state->grid->dimensions.x + trace.x);
            trace = state->explored[id];
        }

        // Add source to the path
        da_ensure_capacity((void *)&state->path, 1);
        state->path[da_length(state->path)] = state->src;
        da_increment_length(state->path);

        // Reverse the path to go from source to target
        size_t path_len = da_length(state->path);
        for (size_t i = 0; i < path_len / 2; i++)
        {
            Point temp = state->path[i];
            state->path[i] = state->path[path_len - 1 - i];
            state->path[path_len - 1 - i] = temp;
        }

        return false; // Done - found path
    }

    Point neighbors[4];
    size_t num_neighbors = grid_neighbors(state->grid, cur, neighbors);
    for (size_t i = 0; i < num_neighbors; i++)
    {
        const Point next = neighbors[i];
        id = (size_t)(next.y * state->grid->dimensions.x + next.x);

        if (state->explored[id].x == -1)
        {
            state->explored[id] = cur;

            da_ensure_capacity((void *)&state->queue, 1);
            size_t l = da_length(state->queue);
            state->queue[l] = next;
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
