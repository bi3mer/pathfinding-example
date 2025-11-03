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
        const float tc = terrain_cost(state->grid->grid[id]);
        const float cost = tc + state->cost_so_far[node_id];

        if (state->came_from[id].x == -1 || cost < state->cost_so_far[id])
        {
            const size_t n_id = grid_index(state->grid, neighbors[i]);
            state->came_from[n_id] = node.point;
            state->cost_so_far[n_id] = cost;

            const float priority =
                cost + heuristic(state->heuristic, neighbors[i], state->tgt);

            AStarNode *a = da_priority_insert((void **)&state->queue, priority,
                                              astar_compare);
            a->priority = priority;
            a->point = neighbors[i];
        }
    }

    return true; // Still processing}
}