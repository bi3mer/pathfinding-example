#include "grid.h"
#include <stdlib.h>

extern float stb_perlin_noise3(float x, float y, float z, int x_wrap,
                               int y_wrap, int z_wrap);

void grid_init(Grid *grid, Point dimensions)
{
    grid->dimensions = dimensions;
    grid->grid = malloc(sizeof(Terrain_Type) * (size_t)dimensions.x *
                        (size_t)dimensions.y);
}

void grid_cleanup(Grid *grid)
{
    if (grid->grid)
    {
        free(grid->grid);
        grid->grid = NULL;
    }
}

void grid_perlin_terrain(Grid *grid, float scale, Point offset)
{
    for (int y = 0; y < grid->dimensions.y; y++)
    {
        for (int x = 0; x < grid->dimensions.x; x++)
        {
            float noiseValue = stb_perlin_noise3(
                (x + offset.x / 100.0f) / scale,
                (y + offset.y / 100.0f) / scale, 0.0f, 0, 0, 0);

            grid->grid[y * grid->dimensions.x + x] =
                noise_to_terrain((noiseValue + 1.0f) * 0.5f);
        }
    }
}

Terrain_Type grid_at(Grid *grid, int x, int y)
{
    return grid->grid[y * grid->dimensions.x + x];
}

void grid_set(Grid *grid, int x, int y, Terrain_Type value)
{
    grid->grid[y * grid->dimensions.x + x] = value;
}
