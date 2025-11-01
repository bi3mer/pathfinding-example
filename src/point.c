#include "point.h"
#include <math.h>
#include <stdlib.h>

bool point_equals(const Point a, const Point b)
{
    return a.x == b.x && a.y == b.y;
}

float manhattan_distance(const Point a, const Point b)
{
    return (float)(abs(a.x - b.x) + abs(a.y - b.y));
}

float euclidian_distance(const Point a, const Point b)
{
    const float x = a.x - b.x;
    const float y = a.y - b.y;

    return sqrtf(x * x + y * y);
}

float overweighted_manhattan_distance(const Point a, const Point b)
{
    return 2.f * manhattan_distance(a, b);
}

float inadmissable_heuristic(const Point a, const Point b)
{
    return 100000;
}
