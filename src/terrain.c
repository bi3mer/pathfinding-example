#include "terrain.h"

Terrain_Type noise_to_terrain(float noise)
{
    if (noise < 0.3f)
        return TERRAIN_WATER_DEEP;
    else if (noise < 0.4f)
        return TERRAIN_WATER_SHALLOW;
    else if (noise < 0.5f)
        return TERRAIN_SAND;

    return TERRAIN_GRASS;
}

Color terrain_to_color(Terrain_Type t)
{
    switch (t)
    {
    case TERRAIN_WATER_DEEP:
        return (Color){0, 60, 100, 255};

    case TERRAIN_WATER_SHALLOW:
        return (Color){0, 100, 150, 255};

    case TERRAIN_SAND:
        return (Color){238, 203, 173, 255};

    case TERRAIN_GRASS:
        return (Color){50, 150, 50, 255};

    case TERRAIN_COUNT:
    default:
        return MAGENTA; // Error
    }

    return MAGENTA; // fallback error
}

float terrain_cost(Terrain_Type t)
{
    switch (t)
    {
    case TERRAIN_GRASS:
        return 1.0f;
    case TERRAIN_SAND:
        return 2.0f;
    case TERRAIN_WATER_SHALLOW:
        return 4.0f;
    case TERRAIN_WATER_DEEP:
        return 100.0f;
    case TERRAIN_COUNT:
    default:
        return 100.0f;
    }
    return 100.0f;
}
