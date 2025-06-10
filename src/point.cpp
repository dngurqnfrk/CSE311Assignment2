#include "point.h"

point getPoint(std::string line) {
    //std::cout << "DEBUG (input - getPosition) line is " << line << std::endl;
    std::istringstream iss(line);
    std::string temp;
    point p;
    iss >> temp;
    iss >> temp;
    p.x = stod(temp);
    iss >> temp;
    p.y = stod(temp);

    return p;
}

double pointDistance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}