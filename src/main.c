#include "astar.h"
#include "da.h"
#include "grid.h"
#include "pathfinding.h"
#include "point.h"
#include "raylib.h"
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stddef.h>
#include <time.h>

typedef enum
{
    ALGORITHM_NONE,
    ALGORITHM_BFS,
    ALGORITHM_DFS,
    ALGORITHM_ASTAR
} Algorithm;

int main(void)
{
    Algorithm current_algorithm;
    bool show_help, show_info;
    size_t animated_path_length, i;
    int x, y;
    unsigned int seed;

    Grid grid;
    PathfindingState path_state;
    AStarState astar_state;

    current_algorithm = ALGORITHM_NONE;
    show_help = false;
    show_info = false;
    animated_path_length = 1;
    seed = 1;

    const int grid_size = 40;
    const int cell_size = 20;
    const int screen_width = grid_size * cell_size;
    const int screen_height = grid_size * cell_size;
    const Point src = {5, 5};
    const Point tgt = {grid_size - 5, grid_size - 5};

    // set up grid that we will be pathfinding on
    SetRandomSeed(seed);
    grid_init(&grid, (Point){grid_size, grid_size});
    grid_perlin_terrain(
        &grid, 20.f, (Point){GetRandomValue(0, 100), GetRandomValue(0, 100)});

    // set up pathfiding state
    pathfinding_init(&path_state, &grid, src, tgt);
    astar_state_init(&astar_state, &grid, src, tgt,
                     point_inadmissable_heuristic);

    // raylib set up
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "Pathfinding Example");
    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        /////////////////////// Update ///////////////////////
        Point *came_from;
        Point *path;
        bool path_found;
        switch (current_algorithm)
        {
        case ALGORITHM_NONE:
            came_from = NULL;
            path = NULL;
            path_found = false;
            break;
        case ALGORITHM_BFS:
            if (!path_state.found)
                bfs_step(&path_state);

            came_from = path_state.came_from;
            path = path_state.path;
            path_found = path_state.found;

            break;
        case ALGORITHM_DFS:
            if (!path_state.found)
                dfs_step(&path_state);

            came_from = path_state.came_from;
            path = path_state.path;
            path_found = path_state.found;

            break;
        case ALGORITHM_ASTAR:
            if (!astar_state.found)
                astar_step(&astar_state);

            came_from = astar_state.came_from;
            path = astar_state.path;
            path_found = astar_state.found;

            break;
        default:
            printf("Unhandled state type: %u\n", current_algorithm);
            exit(1);
        }

        /////////////////////// Draw ///////////////////////
        BeginDrawing();
        ClearBackground(BLACK);

        for (y = 0; y < grid_size; y++)
        {
            for (x = 0; x < grid_size; x++)
            {
                DrawRectangle(x * cell_size, y * cell_size, cell_size,
                              cell_size,
                              terrain_to_color(grid_at(&grid, (Point){x, y})));

                int idx = y * grid.dimensions.x + x;
                if (came_from && came_from[idx].x != -1)
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

        if (path && path_found)
        {
            const size_t path_length = da_length(path) - 1;
            for (i = 0; i < animated_path_length && i < path_length; i++)
            {
                Vector2 start = {path[i].x * cell_size + cell_size / 2,
                                 path[i].y * cell_size + cell_size / 2};

                Vector2 end = {path[i + 1].x * cell_size + cell_size / 2,
                               path[i + 1].y * cell_size + cell_size / 2};

                DrawLineEx(start, end, 3.0f, YELLOW);
            }

            animated_path_length = animated_path_length < path_length
                                       ? animated_path_length + 1
                                       : path_length;
        }

        /////////////////////////////////////////
        // Informatino Panel
        if (IsKeyPressed(KEY_I))
        {
            show_info = !show_info;
        }

        if (show_info)
        {
            size_t explored_cells = 0;
            float path_cost = 0.0f;
            size_t path_length = 0;

            if (came_from)
            {
                for (i = 0; i < grid_size * grid_size; ++i)
                {
                    if (came_from[i].x != -1)
                    {
                        explored_cells++;
                    }
                }
            }

            if (path)
            {
                path_length = da_length(path);
                for (i = 0; i < path_length; ++i)
                {
                    path_cost += terrain_cost(grid_at(&grid, path[i]));
                }
            }

            int panel_width =
                250; // Fixed width that fits "Nodes explored: 99999"
            int panel_height = 100;
            int info_x = screen_width - panel_width - 10;
            int info_y = 10;
            int line_height = 25;

            DrawRectangle(info_x - 5, info_y - 5, panel_width + 10,
                          panel_height, (Color){0, 0, 0, 180});

            const char *algo_name = "None";
            switch (current_algorithm)
            {
            case ALGORITHM_BFS:
                algo_name = "BFS";
                break;
            case ALGORITHM_DFS:
                algo_name = "DFS";
                break;
            case ALGORITHM_ASTAR:
                algo_name = "A*";
                break;
            default:
                break;
            }

            DrawText(TextFormat("Nodes explored: %zu", explored_cells), info_x,
                     info_y, 20, WHITE);
            DrawText(TextFormat("Path length: %zu", path_length), info_x,
                     info_y + line_height, 20, WHITE);
            DrawText(TextFormat("Path cost: %.1f", path_cost), info_x,
                     info_y + line_height * 2, 20, WHITE);
            DrawText(TextFormat("Algorithm: %s", algo_name), info_x,
                     info_y + line_height * 3, 20, WHITE);
        }

        /////////////////////////////////////////
        // Algorithm Selection
        if (IsKeyPressed(KEY_H))
        {
            show_help = !show_help;
        }

        if (show_help)
        {
            DrawRectangle(0, 0, screen_width, screen_height,
                          (Color){0, 0, 0, 180});

            GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

            int button_height = 60;
            int button_padding = 40; // Padding inside button
            int button_spacing = 20; // Space between buttons

            // Find the widest text
            int bfs_text_width = MeasureText("BFS", 24);
            int dfs_text_width = MeasureText("DFS", 24);
            int astar_text_width = MeasureText("A*", 24);

            int max_text_width = bfs_text_width;
            if (dfs_text_width > max_text_width)
                max_text_width = dfs_text_width;
            if (astar_text_width > max_text_width)
                max_text_width = astar_text_width;

            int button_width = max_text_width + button_padding;

            int total_width = (button_width * 3) + (button_spacing * 2);
            int start_x = (screen_width - total_width) / 2;
            int button_y = screen_height / 2 - button_height / 2;

            if (GuiButton(
                    (Rectangle){start_x, button_y, button_width, button_height},
                    "BFS"))
            {
                current_algorithm = ALGORITHM_BFS;
                pathfinding_cleanup(&path_state);
                pathfinding_init(&path_state, &grid, src, tgt);
                animated_path_length = 1;
                show_help = false;
            }

            if (GuiButton((Rectangle){start_x + button_width + button_spacing,
                                      button_y, button_width, button_height},
                          "DFS"))
            {
                current_algorithm = ALGORITHM_DFS;
                pathfinding_cleanup(&path_state);
                pathfinding_init(&path_state, &grid, src, tgt);
                animated_path_length = 1;
                show_help = false;
            }

            if (GuiButton((Rectangle){start_x + (button_width * 2) +
                                          (button_spacing * 2),
                                      button_y, button_width, button_height},
                          "A*"))
            {
                current_algorithm = ALGORITHM_ASTAR;
                astar_state_cleanup(&astar_state);
                astar_state_init(&astar_state, &grid, src, tgt,
                                 point_euclidian_distance);
                animated_path_length = 1;
                show_help = false;
            }

            // seed and map generation
            int seed_y = button_y + button_height + 40;
            int seed_width = 300;
            int seed_x = (screen_width - seed_width) / 2;

            // Display current seed
            DrawText(TextFormat("Seed: %u", seed), seed_x + seed_width / 2 - 50,
                     seed_y, 20, WHITE);

            // New Random Seed button
            if (GuiButton((Rectangle){seed_x + seed_width / 2 - 60, seed_y + 30,
                                      120, 30},
                          "New Map"))
            {
                seed = GetRandomValue(0, 9999);

                // Regenerate terrain with new seed
                SetRandomSeed(seed);
                grid_cleanup(&grid);
                grid_init(&grid, (Point){grid_size, grid_size});
                grid_perlin_terrain(
                    &grid, 20.f,
                    (Point){GetRandomValue(0, 1000), GetRandomValue(0, 1000)});

                // Reset pathfinding
                if (current_algorithm == ALGORITHM_BFS ||
                    current_algorithm == ALGORITHM_DFS)
                {
                    pathfinding_cleanup(&path_state);
                    pathfinding_init(&path_state, &grid, src, tgt);
                }
                else if (current_algorithm == ALGORITHM_ASTAR)
                {
                    astar_state_cleanup(&astar_state);
                    astar_state_init(&astar_state, &grid, src, tgt,
                                     point_euclidian_distance);
                }
                animated_path_length = 1;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
