#ifndef _HEURISTIC_
#define _HEURISTIC_

#include "point.h"

typedef enum
{
    Heuristic_Manhattan = 0,
    Heuristic_Inadmissable_Manhattan,
    Heuristic_Euclidian,
    Heuristic_Zero,
    Heuristic_Count
} Heuristic;

extern float heuristic(Heuristic h, const Point a, const Point b);

#endif
