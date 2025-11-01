#include "astar.h"
#include "da.h"
#include "grid.h"
#include "point.h"

#include <math.h>
#include <stdbool.h>
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
    if (state->found || da_length(state->queue) == 0)
        return false; // done processing

    const AStarNode node = state->queue[0];
    const size_t node_id = grid_index(state->grid, node.point);
    da_pop_start(state->queue);

    if (point_equals(node.point, state->tgt))
    {
        Point cur = node.point;
        state->found = true;

        while (!point_equals(cur, state->src))
        {
            *(Point *)da_append((void **)&state->path) = cur;
            cur = state->came_from[grid_index(state->grid, cur)];
        }

        *(Point *)da_append((void **)&state->path) = cur;
        da_reverse(state->path);

        return false; // done processing
    }

    Point neighbors[4];
    const size_t num_neighbors =
        grid_neighbors(state->grid, node.point, neighbors);

    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const Point n = neighbors[i];
        const size_t id = grid_index(state->grid, n);

        const Terrain_Type terrain = grid_at(state->grid, n);
        const float move_cost = terrain_cost(terrain);

        const float new_cost = state->cost_so_far[node_id] + move_cost;

        if (state->came_from[id].x == -1 || new_cost < state->cost_so_far[id])
        {
            state->cost_so_far[id] = new_cost;
            state->came_from[id] = node.point;

            const float priority = new_cost + state->heuristic(n, state->tgt);
            const size_t insert_idx = da_priority_insert(
                (void **)&state->queue, (size_t)priority, astar_compare);

            state->queue[insert_idx] = (AStarNode){n, priority};
        }
    }

    return true;
}

bool astar(AStarState *state)
{
    while (astar_step(state))
    {
    }

    return state->found;
}
