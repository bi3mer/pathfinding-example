#ifndef _POINT_
#define _POINT_

#include <stdbool.h>

typedef struct Point
{
    int x, y;
} Point;

bool point_equals(const Point a, const Point b);

#endif
