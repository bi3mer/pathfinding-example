#include "point.h"
#include <stdlib.h>

bool point_equals(const Point a, const Point b)
{
    return a.x == b.x && a.y == b.y;
}
