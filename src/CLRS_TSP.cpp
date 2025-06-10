#include "CLRS_TSP.h"


void preorderTraversal(vertex* v, std::vector<vertex*>& probePosition) {
    probePosition.emplace_back(v);

    for (unsigned i = 0; i < v->getChildren().size(); i++) {
        preorderTraversal(v->getChildren()[i], probePosition);
    }
}

void CLRS_TSP(std::vector<vertex> positions, unsigned dimension) {
    prim(positions);
    std::vector<vertex*> probePosition;

    double total_distance = 0.0f;
    unsigned vertexCount = 1;
    vertex *probeV = &positions[1];

    /*
    //unsigned debug = 0;
    while (vertexCount < dimension) {
        bool probeAllChildren = true;

        probeV->setVisit(true);
        std::vector<vertex*> children = probeV->getChildren();
        for (unsigned i = 0; i < children.size(); i++) {
            if (children[i]->getVisited()) {
                continue;
            }
            probeAllChildren = false;
            vertexCount++;
            total_distance += pointDistance(probeV->getPoint(), children[i]->getPoint());
            probePosition.emplace_back(children[i]);
            probeV = children[i];
            break;
        }

        if (probeAllChildren) {
            if (probeV->getParent() != NULL)
                probeV = probeV->getParent();
        }
    }
    */

    preorderTraversal(probeV, probePosition);

    for (unsigned i = 0; i < probePosition.size(); i++) {
        total_distance += pointDistance(probeV->getPoint(), probePosition[i]->getPoint());
        printf("[%5d] ", probePosition[i]->getIndex());
        if ((i + 1) % 8 == 0)
            printf("\n");

        probeV = probePosition[i];
    }

    total_distance += pointDistance(probeV->getPoint(), probePosition[0]->getPoint());

    std::cout << std::endl;
    printf("Total Distance : %f\n", total_distance);
}

std::vector<unsigned> getCLRS_TSP(std::vector<vertex> positions, unsigned dimension) {
    prim(positions);
    std::vector<vertex*> probePosition;

    double total_distance = 0.0f;
    unsigned vertexCount = 1;
    vertex *probeV = &positions[1];

    preorderTraversal(probeV, probePosition);

    std::vector<unsigned> CLRS_permutation;

    for (unsigned i = 0; i < probePosition.size(); i++) {
        int index = probeV->getIndex();
        for (unsigned j = 1; j < positions.size(); j++) {
            if (positions[j].getIndex() == index) {
                CLRS_permutation.emplace_back(j);
                break;
            }
        }
        probeV = probePosition[i];
    }
    
    return CLRS_permutation;
}