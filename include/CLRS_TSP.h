#pragma once

#include <iostream>

#include "graph.h"

void preorderTraversal(vertex* v, std::vector<vertex*>& probePosition);
void CLRS_TSP(std::vector<vertex> positions, unsigned dimension);
std::vector<unsigned> getCLRS_TSP(std::vector<vertex> positions, unsigned dimension);