#include "heuristic.h"
#include <math.h>
#include <stdlib.h>

float heuristic(Heuristic h, const Point a, const Point b)
{
    float res = 0;
    switch (h)
    {
    case Heuristic_Manhattan:
    {

        res = (float)(abs(a.x - b.x) + abs(a.y - b.y));
        break;
    }
    case Heuristic_Inadmissable_Manhattan:
    {
        res = 4.f * (float)(abs(a.x - b.x) + abs(a.y - b.y));
        break;
    }
    case Heuristic_Euclidian:
    {
        const float x = a.x - b.x;
        const float y = a.y - b.y;

        res = sqrtf(x * x + y * y);
    }
    case Heuristic_Zero:
    case Heuristic_Count:
    default:
        break;
    }

    return res;
}
