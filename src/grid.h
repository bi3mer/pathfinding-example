#ifndef _GRID_
#define _GRID_

#include "point.h"
#include "terrain.h"
#include <stddef.h>

typedef struct Grid
{
    Terrain_Type *grid;
    Point dimensions;
} Grid;

extern void grid_init(Grid *grid, Point dimensions);
extern void grid_cleanup(Grid *grid);

extern void grid_perlin_terrain(Grid *grid, float scale, Point offset);

extern Terrain_Type grid_at(Grid *grid, Point p);
extern void grid_set(Grid *grid, Point p, Terrain_Type value);

extern size_t grid_neighbors(Grid *grid, Point p, Point out[4]);

#endif
