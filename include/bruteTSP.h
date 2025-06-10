#pragma once

#include "graph.h"
#include <iostream>
#include <list>
#include <algorithm>


void bruteTSP(std::vector<vertex> positions, int dimension) {
    int permu[dimension - 1];
    for (unsigned i = 0; i < dimension - 1; i++) {
        permu[i] = i + 2;
    }

    int ans_permu[dimension - 1];

    double ans_distance = INT_MAX;

    do {
        vertex* probe = &positions[1];
        double total_distance = 0.0f;

        for(int i = 0; i < dimension - 1; i++) {
            total_distance += pointDistance(probe->getPoint(), positions[permu[i]].getPoint());

            probe = &positions[permu[i]];
        }
        total_distance += pointDistance(probe->getPoint(), positions[1].getPoint());
    
        if (total_distance < ans_distance) {
            ans_distance = total_distance;
            for(int i = 0; i < dimension - 1; i++) {
                ans_permu[i] = permu[i];
            }
        }
    } while(std::next_permutation(permu, permu + dimension - 2));

    printf("[    1] ");
    for(unsigned i = 0; i < dimension - 1; i++) {
        printf("[%5d] ", ans_permu[i]);
        if((i + 2) % 8 == 0)
            std::cout << std::endl;
    }

    std::cout << "\nTotal distance: " << ans_distance << std::endl;
}