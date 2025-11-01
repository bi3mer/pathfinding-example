#include "astar.h"
#include <stdlib.h>

int astar_compare(const void *node, const size_t priority)
{
    const AStarNode *n = (const AStarNode *)node;
    return (n->f_score > priority) ? 1 : -1;
}

float heuristic(Point a, Point b)
{
    return (float)(abs(a.x - b.x) + abs(a.y - b.y));
}

AStarState *astar_state_init(Grid *grid, Point src, Point tgt)
{
    AStarState *state = calloc(1, sizeof(AStarState));
    state->grid = grid;
    state->src = src;
    state->tgt = tgt;

    const size_t grid_size = grid->dimensions.x * grid->dimensions.y;

    state->open_set = da_init(sizeof(AStarNode), 16);
    state->path = da_init(sizeof(Point), 16);
    state->came_from = malloc(sizeof(Point) * grid_size);
    state->g_score = malloc(sizeof(float) * grid_size);
    state->closed_set = calloc(grid_size, sizeof(bool));

    // Initialize arrays
    for (size_t i = 0; i < grid_size; i++)
    {
        state->came_from[i] = (Point){-1, -1};
        state->g_score[i] = INFINITY;
    }

    // Add starting node
    state->g_score[grid_index(grid, src)] = 0;
    AStarNode start = {src, heuristic(src, tgt)};
    *(AStarNode *)da_append((void **)&state->open_set) = start;

    state->found = false;
    return state;
}

bool astar_step(AStarState *state)
{
    if (state->found || da_length(state->open_set) == 0)
        return false;

    // Get node with lowest f_score
    AStarNode current = state->open_set[0];
    da_pop_start(state->open_set);

    const size_t cur_id = grid_index(state->grid, current.point);

    // Skip if already evaluated
    if (state->closed_set[cur_id])
        return true;

    state->closed_set[cur_id] = true;

    // Check if we reached the target
    if (point_equals(current.point, state->tgt))
    {
        state->found = true;

        // Reconstruct path
        Point p = state->tgt;
        while (!point_equals(p, state->src))
        {
            *(Point *)da_append((void **)&state->path) = p;
            p = state->came_from[grid_index(state->grid, p)];
        }
        *(Point *)da_append((void **)&state->path) = state->src;
        da_reverse(state->path);
        return false;
    }

    // Check neighbors
    Point neighbors[4];
    const size_t num_neighbors =
        grid_neighbors(state->grid, current.point, neighbors);

    for (size_t i = 0; i < num_neighbors; ++i)
    {
        const Point n = neighbors[i];
        const size_t n_id = grid_index(state->grid, n);

        if (state->closed_set[n_id])
            continue;

        float tentative_g = state->g_score[cur_id] + 1.0f; // Cost of 1 per move

        if (tentative_g < state->g_score[n_id])
        {
            state->came_from[n_id] = current.point;
            state->g_score[n_id] = tentative_g;

            float f = tentative_g + heuristic(n, state->tgt);

            // Add to open set with priority
            size_t insert_idx = da_priority_insert((void **)&state->open_set,
                                                   (size_t)f, astar_compare);
            state->open_set[insert_idx] = (AStarNode){n, f};
        }
    }

    return true;
}

void astar_state_cleanup(AStarState *state)
{
    if (state)
    {
        da_cleanup(state->open_set);
        da_cleanup(state->path);
        free(state->came_from);
        free(state->g_score);
        free(state->closed_set);
        free(state);
    }
}