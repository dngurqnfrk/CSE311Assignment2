#pragma once

#include <iostream>
#include <float.h>
#include <algorithm>

#include "graph.h"
#include "CLRS_TSP.h"

typedef struct distPoint_t {
    double distance;
    unsigned point_index;
} distPoint;

typedef struct dpAns_t {
    bool change;
    bool change_mode;
    double distance;
    std::vector<unsigned> permutation;
} dpAns;

bool isInclude(unsigned bit_index, int n);
int countBits(unsigned val);
std::vector<unsigned> range(int num);
unsigned countSlant(std::vector<std::vector<double>>& block_distance_matrix);
std::vector<std::vector<double>> buildBlockDistanceMatrix(std::vector<std::vector<vertex>>& _2D_vertex_vec);
std::vector<std::vector<double>> buildSlantBlockDistanceMatrix(std::vector<std::vector<vertex>>& _2D_vertex_vec);
distPoint find_minimum(std::vector<std::vector<vertex>> _2D_vertex_vec, std::vector<vertex> vertex_vec, std::vector<std::vector<float>>& distance_matrix, std::vector<std::vector<unsigned>>& prev_point_matrix, std::vector<std::vector<double>>& block_distance_matrix, unsigned end_point, unsigned S);
double distanceVector(std::vector<vertex> ver1, std::vector<vertex> ver2);
std::vector<unsigned> produceCombination(int total_size, int num);
std::vector<unsigned> extractPermutation(std::vector<std::vector<unsigned>> prev_point_matrix);
double addUpVertex(std::vector<vertex> vertex_vec);
std::vector<std::vector<vertex>> localDivide(std::vector<vertex> vertex_vec, unsigned u, double slant_threshold_ratio);
std::vector<std::vector<vertex>> globalDivide(std::vector<vertex> vertex_vec, unsigned u);
dpAns HeldKarp(std::vector<std::vector<vertex>>& _2D_vertex_vec, std::vector<vertex> vertex_vec);
void HeldKarpTSP(std::vector<vertex> positions, int dimension);
void appHeldKarpTSP(std::vector<vertex> positions, int dimension);
std::vector<vertex> reshapeVertexVector(vertex start_vertex, std::vector<std::vector<vertex>> _2D_vertex_vec, std::vector<unsigned> permutation);

// DEBUG Func
void printMatrix(std::vector<std::vector<vertex>> _2D_vertex_vec);