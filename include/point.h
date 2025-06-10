#pragma once

#include <string>
#include <cmath>
#include <sstream>

typedef struct point_t {
    double x;
    double y;
} point;

point getPoint(std::string line);
double pointDistance(point p1, point p2);