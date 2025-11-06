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