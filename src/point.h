#ifndef _POINT_
#define _POINT_

#include <stdbool.h>

typedef struct Point
{
    int x, y;
} Point;

extern bool point_equals(const Point a, const Point b);

extern float manhattan_distance(const Point a, const Point b);
extern float euclidian_distance(const Point a, const Point b);

extern float overweighted_manhattan_distance(const Point a, const Point b);
extern float inadmissable_heuristic(const Point a, const Point b);

#endif
