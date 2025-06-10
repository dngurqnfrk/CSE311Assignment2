#include "input.h"
#include "CLRS_TSP.h"
#include "bruteTSP.h"
#include "HeldKarpTSP.h"
#include "ClusterTSP.h"

#include <chrono>

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        std::cout << "argv [" << i << "] : " << argv[i] << std::endl; 

    if (argc > 2) {
        std::cout << "ERROR : need only one argument.\n";
        return -1;
    }

    std::string filename = "input/" + (std::string)argv[1];
    TSPReader r(filename);

    std::vector<vertex> pos = r.getPosition();
    unsigned dimenstion = r.getDimension();

    double initial_cost = 0.0f;
    for(unsigned i = 1; i < pos.size(); i++) {
        if (i == pos.size() - 1)
            initial_cost += pointDistance(pos[i].getPoint(), pos[0].getPoint());
        else
            initial_cost += pointDistance(pos[i].getPoint(), pos[i + 1].getPoint());
    }
    std::cout << "Initial tour cost is ";
    printf("%f\n", initial_cost);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> exec_time = end - start;

    //std::cout << "\n\nbruteTSP\n\n";
    //bruteTSP(r.getPosition(), r.getDimension());

    std::cout << "\n\nCLRS TSP\n\n";
    start = std::chrono::high_resolution_clock::now();
    CLRS_TSP(r.getPosition(), r.getDimension());  
    end = std::chrono::high_resolution_clock::now();
    exec_time = end - start;
    std::cout << "TSP is solved in " << exec_time.count() << "(s) time.\n";
    
    //std::cout << "\n\nHeld-Karp TSP\n\n";
    //HeldKarpTSP(r.getPosition(), r.getDimension());

    std::cout << "\n\nHeld-Karp Approximation TSP\n\n";
    start = std::chrono::high_resolution_clock::now();
    appHeldKarpTSP(pos, dimenstion);
    end = std::chrono::high_resolution_clock::now();
    exec_time = end - start;
    std::cout << "TSP is solved in " << exec_time.count() << "(s) time.\n";

    //std::cout << "\n\nCluster-Based TSP\n\n";
    start = std::chrono::high_resolution_clock::now();
    clusterTSP(pos, dimenstion);
    end = std::chrono::high_resolution_clock::now();
    exec_time = end - start;
    std::cout << "TSP is solved in " << exec_time.count() << "(s) time.\n";
}