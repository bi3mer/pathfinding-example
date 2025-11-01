#ifndef _TERRAIN_
#define _TERRAIN_

#include "raylib.h"
typedef enum
{
    TERRAIN_WATER_DEEP,
    TERRAIN_WATER_SHALLOW,
    TERRAIN_SAND,
    TERRAIN_GRASS,
    TERRAIN_COUNT
} Terrain_Type;

extern Terrain_Type noise_to_terrain(float noise);
extern Color terrain_to_color(Terrain_Type t);
extern float terrain_cost(Terrain_Type t);

#endif
