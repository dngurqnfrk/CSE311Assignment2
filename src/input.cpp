#include "input.h"
#include <stdio.h>

TSPReader::TSPReader (const std::string& filename) {
    this->filename = filename;
    
    readFile();
}

bool TSPReader::readFile () {
    std::ifstream file(filename);
    std::string line;

    if(!file.is_open()) {
        std::cout << "ERROR : file " << filename << " is not found.\n";
        return false;
    }

    while (std::getline(file, line)) {
        //std::cout << line << std::endl;
        if (line == "NODE_COORD_SECTION")
            break;

        std::string *split_string = stringSplit(line, ':');
        //std::cout << "(TSPreader)Front : " << split_string[0] << std::endl;
        
        if (split_string[0] == "NAME")
            this->filename = split_string[1];

        if (split_string[0] == "DIMENSION") {
            //std::cout << "dimenstion : " << split_string[1] << std::endl;
            this->dimension = stoi(split_string[1]);
        }
    }

    std::cout << "filename : " << this->filename << std::endl;
    std::cout << "dimension : " << this->dimension << std::endl;

    point zero = {0, 0};
    this->position.emplace_back(vertex(zero, 0));
    int vertex_index = 1;
    while (std::getline(file, line)) {
        if (line == "EOF")
            break;

        point tempP = getPoint(line);
        this->position.emplace_back(vertex(tempP, vertex_index));
        vertex_index++;
    }

    for(unsigned i = 0; i < this->position.size(); i++) {
        point p0 = this->position[i].getPoint();
    }

    file.close();
    return true;
}

std::string* stringSplit(std::string line, const char pivot) {
    std::string *split_string = new std::string[2];

    for (unsigned i = 0; i < line.length(); i++) {
        if (line[i] == ':') {
            //std::cout << "DEBUG (input - stringSplit) line is " << line << std::endl;
            std::string temp = line;
            temp.erase(i, line.length());
            std::istringstream iss(temp);
            iss >> split_string[0];
            iss.clear();
            temp = line;
            temp.erase(0, i + 1);
            iss.str(temp);
            iss >> split_string[1];
            //std::cout << "DEBUG (input - stringSplit) front :" << split_string[0] << std::endl;
            //std::cout << "DEBUG (input - stringSplit) back :" << split_string[1] << std::endl;

            return split_string;
        }
    }

    return NULL;
}