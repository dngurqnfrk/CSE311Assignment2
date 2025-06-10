#pragma once

#include <vector>
#include <bitset>

#include "point.h"

#define INT_MAX 2147483647

class vertex {
private:
    point p;
    double distance;
    bool visited;
    int index;
    vertex *parent;
    std::vector<vertex*> children;
public:
    vertex() : p({0, 0}), distance(INT_MAX), visited(false), index(0), parent(NULL) {}
    vertex(point _p, int i) :p(_p), distance(INT_MAX), visited(false), index(i), parent(NULL) {}
    void setDistance(double d) { this->distance = d; }
    double getDistance() { return this->distance; }
    void setPoint(point _p) { this->p = _p; }
    point getPoint() { return (this->p); }
    void setVisit(bool v) { this->visited = v; }
    bool getVisited() { return visited; }
    int getIndex() { return index; }
    vertex* getParent() { return parent; }
    void setParent(vertex *v) { this->parent = v; }
    std::vector<vertex*> getChildren() { return this->children; }
    void emplaceChild(vertex *v) { this->children.emplace_back(v); }
};

void clearVisited(std::vector<vertex> vec);
void clearVisited(std::vector<vertex*> vec);

bool vectorComparator(vertex* v1, vertex* v2);
void setDistance(vertex* v, double d);
void prim(std::vector<vertex>& position);