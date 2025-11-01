#ifndef _POINT_
#define _POINT_

#include <stdbool.h>

typedef struct Point
{
    int x, y;
} Point;

extern bool point_equals(const Point a, const Point b);

extern float point_manhattan_distance(const Point a, const Point b);
extern float point_euclidian_distance(const Point a, const Point b);

extern float point_overweighted_manhattan_distance(const Point a,
                                                   const Point b);
extern float point_inadmissable_heuristic(const Point a, const Point b);

#endif
