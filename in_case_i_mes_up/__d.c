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