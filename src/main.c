#include "da.h"
#include "grid.h"
#include "pathfinding.h"
#include "point.h"
#include "raylib.h"

#include <stddef.h>
#include <time.h>

int main(void)
{
    Grid grid;
    PathfindingState path_state;

    const int grid_size = 20;
    const int cell_size = 10;
    const int screen_width = grid_size * cell_size;
    const int screen_height = grid_size * cell_size;
    const Point src = {5, 5};
    const Point tgt = {grid_size - 5, grid_size - 5};

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "Pathfinding Example");

    SetTargetFPS(60);

    // set up grid that we will be pathfinding on
    SetRandomSeed(1);
    grid_init(&grid, (Point){grid_size, grid_size});
    grid_perlin_terrain(
        &grid, 20.f, (Point){GetRandomValue(0, 100), GetRandomValue(0, 100)});

    // set up pathfiding state
    pathfinding_init(&path_state, &grid, src, tgt);
    bfs(&path_state);

    size_t animated_path_length = 1;

    while (!WindowShouldClose())
    {
        /////////////////////// Update ///////////////////////
        // if (!path_state.found)
        // {
        //     bfs_step(&path_state);
        // }

        /////////////////////// Draw ///////////////////////
        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < grid_size; y++)
        {
            for (int x = 0; x < grid_size; x++)
            {
                DrawRectangle(x * cell_size, y * cell_size, cell_size,
                              cell_size,
                              terrain_to_color(grid_at(&grid, (Point){x, y})));

                int idx = y * grid.dimensions.x + x;
                if (path_state.explored[idx].x != -1)
                {
                    DrawRectangle(x * cell_size, y * cell_size, cell_size,
                                  cell_size, (Color){100, 0, 0, 100});
                }
            }
        }

        DrawRectangle(src.x * cell_size, src.y * cell_size, cell_size,
                      cell_size, GREEN);
        DrawRectangle(tgt.x * cell_size, tgt.y * cell_size, cell_size,
                      cell_size, RED);

        if (path_state.found)
        {
            for (size_t i = 0;
                 i < animated_path_length && i < da_length(path_state.path) - 1;
                 i++)
            {
                Vector2 start = {
                    path_state.path[i].x * cell_size + cell_size / 2,
                    path_state.path[i].y * cell_size + cell_size / 2};

                Vector2 end = {
                    path_state.path[i + 1].x * cell_size + cell_size / 2,
                    path_state.path[i + 1].y * cell_size + cell_size / 2};

                DrawLineEx(start, end, 3.0f, YELLOW);
            }

            ++animated_path_length;
        }

        EndDrawing();
    }

    CloseWindow();

    size_t explored_cells = 0;
    for (size_t i = 0; i < grid_size * grid_size; ++i)
    {
        explored_cells += path_state.explored[i].x != -1;
    }

    float path_cost = 0;
    const size_t path_length = da_length(path_state.path);
    Point p;
    for (size_t i = 0; i < path_length; ++i)
    {
        p = path_state.path[i];
        path_cost += terrain_to_cost(grid_at(&grid, p));
    }

    printf("Cells explored: %lu\n", explored_cells);
    printf("Path Length: %lu\n", path_length);
    printf("Path Cost: %f\n", path_cost);

    grid_cleanup(&grid);
    pathfinding_cleanup(&path_state);

    return 0;
}
