#include "astar.h"
#include "da.h"
#include "grid.h"
#include "heuristic.h"
#include "point.h"
#include "terrain.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

static int astar_compare(const void *node, const float priority)
{
    const AStarNode *n = (const AStarNode *)node;
    return (n->priority > priority) ? 1 : -1;
}

void astar_state_init(AStarState *state, Grid *grid, Point src, Point tgt,
                      Heuristic heuristic)
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
    start->priority = 0;
    start->point = src;

    state->found = false;
}

void astar_state_cleanup(AStarState *state)
{
    if (state)
    {
        da_cleanup(state->queue);
        state->queue = NULL;

        da_cleanup(state->path);
        state->path = NULL;

        free(state->came_from);
        state->came_from = NULL;

        free(state->cost_so_far);
        state->cost_so_far = NULL;

        state->found = false;
    }
}

bool astar_step(AStarState *state)
{
    if (state->found || da_length(state->queue) == 0)
        return false; // done processing

    AStarNode node = state->queue[0];
    da_pop_start(state->queue);
    const size_t node_id = grid_index(state->grid, node.point);

    if (point_equals(node.point, state->tgt))
    {
        state->found = true;

        Point p = node.point;

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
    const size_t num_neighbors =
        grid_neighbors(state->grid, node.point, neighbors);
    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const size_t id = grid_index(state->grid, neighbors[i]);
        if (state->came_from[id].x == -1)
        {
            state->came_from[id] = node.point;

            AStarNode *n = da_append((void **)&state->queue);
            n->point = neighbors[i];
        }
    }

    return true; // Still processing
}

bool astar(AStarState *state)
{
    while (astar_step(state))
    {
    }

    return state->found;
}
