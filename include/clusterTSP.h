#pragma once

#include <vector>
#include <random>
#include <cfloat>
#include <iostream>

#include "graph.h"
#include "CLRS_TSP.h"
#include "HeldKarpTSP.h"

typedef struct clusterVertex_t {
    int cluster_id;
    vertex v;
} clusterVertex;

std::vector<std::vector<vertex>> kmeansPP(std::vector<vertex>& positions, int k);
int estimateK(std::vector<vertex>& positions, int maxK);
double calculateDistortion(const std::vector<std::vector<vertex>>& clusters, const std::vector<vertex>& positions);
void clusterTSP(std::vector<vertex> positions, int dimension);

// For DEBUG
void printCluster(std::vector<std::vector<vertex>>& _2D_vertex_vec);