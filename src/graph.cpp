#include "priorityQueue.h"
#include "graph.h"

void clearVisited(std::vector<vertex> vec) {
    for (unsigned i = 1; i < vec.size(); i++)
        vec[i].setVisit(false);
}

void clearVisited(std::vector<vertex*> vec) {
    for (unsigned i = 1; i < vec.size(); i++)
        vec[i]->setVisit(false);
}

bool vectorComparator(vertex* v1, vertex* v2) {
    return v1->getDistance() > v2->getDistance();
}

void setDistance(vertex* v, double d) {
    v->setDistance(d);
}

void prim(std::vector<vertex>& position) {
    std::vector<vertex*> vertexVec;
    for (unsigned i = 1; i < position.size(); i++) 
        vertexVec.emplace_back(&position[i]);

    for (unsigned i = 0; i < vertexVec.size(); i++)
        vertexVec[i]->setDistance(INT_MAX);

    vertexVec[0]->setDistance(0);
    heapSort<vertex*> hs(vectorComparator);

    while(!vertexVec.empty()) {
        vertex* v = hs.pop(vertexVec);
        //printf("(prim) v is popped [%d]\n", v->getIndex());
        //printf("size (prim) is %d\n", vertexVec.size());
        for (unsigned i = 0; i < vertexVec.size(); i++) {
            point tempP = vertexVec[i]->getPoint();
            //printf("[%d] %f %f : %f\n", vertexVec[i]->getIndex(), tempP.x, tempP.y, vertexVec[i]->getDistance());
        }

        for (unsigned i = 0; i < vertexVec.size(); i++) {
            double d = pointDistance(v->getPoint(), vertexVec[i]->getPoint());
            if (d < vertexVec[i]->getDistance()) {
                vertexVec[i]->setDistance(d);
                //printf("DEBUG (graph - prim) [%d] become child of [%d]\n", vertexVec[i]->getIndex(), v->getIndex());
                vertexVec[i]->setParent(v);
            }
        }

        hs.sort(vertexVec);
    }

    for (unsigned i = 1; i < position.size(); i++) {
        if (position[i].getParent() != NULL) {
            //printf("DEBUG (graph - prim) [%d] get children [%d]\n", position[i].getParent()->getIndex(), position[i].getIndex());
            position[i].getParent()->emplaceChild(&position[i]);
        }
    }
}