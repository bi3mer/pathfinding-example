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
const size_t num_neighbors = grid_neighbors(state->grid, node.point, neighbors);

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