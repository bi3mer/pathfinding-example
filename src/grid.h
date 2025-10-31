#ifndef _GRID_
#define _GRID_

#include "point.h"
#include "terrain.h"
typedef struct Grid
{
    Terrain_Type *grid; // Dynamic flat array
    Point dimensions;
} Grid;

extern void grid_init(Grid *grid, Point dimensions);
extern void grid_cleanup(Grid *grid);

extern void grid_perlin_terrain(Grid *grid, float scale, Point offset);

extern Terrain_Type grid_at(Grid *grid, int x, int y);
extern void grid_set(Grid *grid, int x, int y, Terrain_Type value);

#endif
