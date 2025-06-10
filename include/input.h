#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "graph.h"
#include "point.h"

class TSPReader {
private:
    std::string filename;
    int dimension;
    std::vector<vertex> position;

public:
    TSPReader(const std::string& filename);
    bool readFile();
    int getDimension() const { return dimension; }
    std::vector<vertex> getPosition() const { return position; }
 };

std::string* stringSplit(std::string line, const char pivot);