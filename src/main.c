#include "raylib.h"
#include "terrain.h"
#include <time.h>

#define GRID_SIZE 100
#define CELL_SIZE 10

extern float stb_perlin_noise3(float x, float y, float z, int x_wrap,
                               int y_wrap, int z_wrap);

typedef struct
{
    int x, y;
} Point;

int main(void)
{
    const int screen_width = GRID_SIZE * CELL_SIZE;
    const int screen_height = GRID_SIZE * CELL_SIZE;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "Pathfinding Example");

    SetTargetFPS(60);

    /////////////////////// Generate Map ///////////////////////
    SetRandomSeed(time(NULL));

    const float offset_x = GetRandomValue(0, 10000) / 100.0f;
    const float offset_y = GetRandomValue(0, 10000) / 100.0f;

    Terrain_Type grid[GRID_SIZE][GRID_SIZE];
    float scale = 20.0f;

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            float noiseValue = stb_perlin_noise3(
                (x + offset_x) / scale, (y + offset_y) / scale, 0.0f, 0, 0, 0);

            grid[y][x] = noise_to_terrain((noiseValue + 1.0f) * 0.5f);
        }
    }

    const Point src = {15, 15};
    const Point tgt = {GRID_SIZE - 15, GRID_SIZE - 15};

    while (!WindowShouldClose())
    {
        /////////////////////// Update ///////////////////////

        /////////////////////// Draw ///////////////////////
        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < GRID_SIZE; y++)
        {
            for (int x = 0; x < GRID_SIZE; x++)
            {
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,
                              CELL_SIZE, terrain_to_color(grid[y][x]));
            }
        }

        DrawRectangle(src.x * CELL_SIZE, src.y * CELL_SIZE, CELL_SIZE,
                      CELL_SIZE, RED);
        DrawRectangle(tgt.x * CELL_SIZE, tgt.y * CELL_SIZE, CELL_SIZE,
                      CELL_SIZE, RED);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
