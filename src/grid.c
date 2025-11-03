#include "grid.h"
#include "point.h"
#include "terrain.h"
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
                (x + offset.x) / scale, (y + offset.y) / scale, 0.0f, 0, 0, 0);

            grid->grid[y * grid->dimensions.x + x] =
                noise_to_terrain((noiseValue + 1.0f) * 0.5f);
        }
    }
}

extern size_t grid_index(const Grid *grid, const Point p)
{
    return (size_t)(p.y * grid->dimensions.x + p.x);
}

Terrain_Type grid_at(Grid *grid, Point p)
{
    return grid->grid[p.y * grid->dimensions.x + p.x];
}

void grid_set(Grid *grid, Point p, Terrain_Type value)
{
    grid->grid[p.y * grid->dimensions.x + p.x] = value;
}

size_t grid_neighbors(Grid *grid, Point p, Point out[4])
{
    size_t count = 0;
    if (p.x > 0 && grid->grid[grid_index(grid, (Point){p.x - 1, p.y})] !=
                       TERRAIN_WATER_SHALLOW)
    {
        out[count].x = p.x - 1;
        out[count].y = p.y;
        ++count;
    }

    if (p.x < grid->dimensions.x - 1 &&
        grid->grid[grid_index(grid, (Point){p.x + 1, p.y})] !=
            TERRAIN_WATER_SHALLOW)
    {
        out[count].x = p.x + 1;
        out[count].y = p.y;
        ++count;
    }

    if (p.y > 0 && grid->grid[grid_index(grid, (Point){p.x, p.y - 1})] !=
                       TERRAIN_WATER_SHALLOW)
    {
        out[count].x = p.x;
        out[count].y = p.y - 1;
        ++count;
    }

    if (p.y < grid->dimensions.y - 1 &&
        grid->grid[grid_index(grid, (Point){p.x, p.y + 1})] !=
            TERRAIN_WATER_SHALLOW)
    {
        out[count].x = p.x;
        out[count].y = p.y + 1;
        ++count;
    }

    return count;
}
