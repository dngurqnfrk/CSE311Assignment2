#include "HeldKarpTSP.h"

bool isInclude(unsigned bit_index, int n) {
    return bit_index & (1 << (n - 1));
}

int countBits(unsigned val) {
    int count = 0;
    while (val) {
        count += val & 1;
        val >>= 1;
    }

    return count;
}

std::vector<unsigned> range(int num) {
    std::vector<unsigned> temp_vec;

    for(unsigned i = 0; i < num + 1; i++) {
        temp_vec.emplace_back(i);
    }

    return temp_vec;
}

unsigned countSlant(std::vector<std::vector<double>>& block_distance_matrix) {
    unsigned size = block_distance_matrix.size();
    unsigned slant_bit = 0;
    for(unsigned i = 0; i < size; i++) {
        bool is_min = true;
        unsigned second_index = i + 1;
        if (i == size - 1)
            second_index = 0;
        double slant_distance = block_distance_matrix[i][second_index];
        for (unsigned j = 0; j < size; j++) {
            if (i != j && i != second_index) {
                if (slant_distance > block_distance_matrix[i][j])
                    is_min = false;
            }
        }

        if (is_min)
            slant_bit = slant_bit | (1 << i);
    }

    for(unsigned i = 0; i < size; i++) {
        bool is_min = true;
        unsigned second_index;
        if (i == 0)
            second_index = size - 1;
        else
            second_index = i - 1;
        double slant_distance = block_distance_matrix[second_index][i];
        for (unsigned j = 0; j < size; j++) {
            if (i != j && i != second_index) {
                if (slant_distance > block_distance_matrix[j][i])
                    is_min = false;
            }
        }

        if (is_min) {
            unsigned or_bit;
            if (i == 0)
                or_bit = (1 << (size - 1));
            else
                or_bit = (1 << (i - 1));

            slant_bit = slant_bit | or_bit;
        }
    }

    return countBits(slant_bit);
}

std::vector<std::vector<double>> buildBlockDistanceMatrix(std::vector<std::vector<vertex>>& _2D_vertex_vec) {
    unsigned size = _2D_vertex_vec.size() - 1;
    std::vector<std::vector<double>> block_distance_matrix;
    for(unsigned i = 0; i < size + 1; i++) {
        std::vector<double> temp_vec;
        for(unsigned j = 0; j < size + 1; j++) {
            double temp_distance = 0.0f;
            if (i != j) {
                temp_distance += distanceVector(_2D_vertex_vec[i], _2D_vertex_vec[j]);
                temp_distance += addUpVertex(_2D_vertex_vec[i]);
            } 

            temp_vec.emplace_back(temp_distance);
        }

        block_distance_matrix.emplace_back(temp_vec);
    }

    return block_distance_matrix;
}

std::vector<std::vector<double>> buildSlantBlockDistanceMatrix(std::vector<std::vector<vertex>>& _2D_vertex_vec) {
    unsigned size = _2D_vertex_vec.size() - 1;
    std::vector<std::vector<double>> block_distance_matrix;
    for(unsigned i = 0; i < size + 1; i++) {
        std::vector<double> temp_vec;
        for(unsigned j = 0; j < size + 1; j++) {
            double temp_distance = 0.0f;
            if (i != j) {
                temp_distance += distanceVector(_2D_vertex_vec[i], _2D_vertex_vec[j]);
            } 

            temp_vec.emplace_back(temp_distance);
        }

        block_distance_matrix.emplace_back(temp_vec);
    }

    return block_distance_matrix;
}

distPoint find_minimum(std::vector<std::vector<vertex>> _2D_vertex_vec, std::vector<vertex> vertex_vec, std::vector<std::vector<float>>& distance_matrix, std::vector<std::vector<unsigned>>& prev_point_matrix, std::vector<std::vector<double>>& block_distance_matrix, unsigned end_point, unsigned S) {
    distPoint min_distPoint = {FLT_MAX, 0};
    unsigned size = _2D_vertex_vec.size() - 1;

    S -= (1 << (end_point - 1));

    if (S == 0) {
        //printf("DEBUG(HeldKarp - find min) S is zero\n");
        min_distPoint = {distance_matrix[end_point][0], 0};
        return min_distPoint;
    }

    for (unsigned i = 1; i < size + 1; i++) {
        if (isInclude(S, i)) {
            //std::cout << "DEBUG " << std::bitset<16>(S) << " has " << i << std::endl;
            std::vector<vertex> u1 = _2D_vertex_vec[i];
            std::vector<vertex> u2 = _2D_vertex_vec[end_point];
            double distance = distance_matrix[i][S] + block_distance_matrix[i][end_point];

            if (distance < min_distPoint.distance) {
                min_distPoint.distance = distance;
                min_distPoint.point_index = i;
            }
        }
    }

    return min_distPoint;
}

double distanceVector(std::vector<vertex> ver1, std::vector<vertex> ver2) {
    return pointDistance(ver1[ver1.size() - 1].getPoint(), ver2[0].getPoint());
}

std::vector<unsigned> produceCombination(int total_size, int num) {
    std::vector<unsigned> ansVec;
    
    unsigned start = (1 << num) - 1;
    unsigned end = 1 << total_size;

    unsigned val = start;
    while(val < end) {
        //printf("DEBUG(HeldKarp - produceComb) total[%d] num[%d] has val [%d]\n", total_size, num, val);
        if (countBits(val) == num) {
            ansVec.emplace_back(val);
        }

        unsigned c = val & -val; // find minmum 1 bit
        unsigned r = val + c;

        val = (((r ^ val) >> 2) / c) | r;
    }

    return ansVec;
}

std::vector<unsigned> extractPermutation(std::vector<std::vector<unsigned>> prev_point_matrix) {
    unsigned probe = 0;
    unsigned probeS = (1 << (prev_point_matrix.size() - 1)) - 1;
    std::vector<unsigned> temp_vec(1, 0);

    while(probeS > 0) {
        unsigned next = prev_point_matrix[probe][probeS];
        /*
        printf("DEBUG(HeldKarp - permu) C[%d][", probe);
        std::cout << std::bitset<16>(probeS) << "] = " << next << std::endl;
        */
        if (probe > 0)
            probeS -= (1 << (probe - 1));
        
        temp_vec.emplace_back(next);
        probe = next;
    }

    reverse(temp_vec.begin(), temp_vec.end());
    return temp_vec;
}

double addUpVertex(std::vector<vertex> vertex_vec) {
    double internal_distance = 0.0f;

    vertex prev = vertex_vec[0];
    for (unsigned i = 1; i < vertex_vec.size(); i++) { 
        internal_distance += pointDistance(prev.getPoint(), vertex_vec[i].getPoint());
        prev = vertex_vec[i];
    }

    return internal_distance;
}


std::vector<std::vector<vertex>> localDivide(std::vector<vertex> vertex_vec, unsigned u, double slant_threshold_ratio) {
    if (vertex_vec.size() - 1 <= u) {
        std::vector<std::vector<vertex>> _2D_vertex_vec;
        for(unsigned i = 1; i < vertex_vec.size(); i++) {
            std::vector<vertex> temp_vec(1, vertex_vec[i]);
            _2D_vertex_vec.emplace_back(temp_vec);
        }

        return _2D_vertex_vec;
    }

    for (unsigned super_index = 1; super_index < u + 1; super_index++) {
        std::vector<std::vector<vertex>> _2D_vertex_vec;
        unsigned super_city_size = vertex_vec.size() - u - 1;
        for (unsigned i = 1; i < vertex_vec.size(); i++) {
            std::vector<vertex> temp_vec;
            temp_vec.emplace_back(vertex_vec[i]);
            if (i - 1 == super_index) {
                for (unsigned j = 1; j < super_city_size; j++) {
                    i++;
                    temp_vec.emplace_back(vertex_vec[i]);
                }
            }
            _2D_vertex_vec.emplace_back(temp_vec);
        }

        std::vector<std::vector<double>> block_distance_matrix = buildSlantBlockDistanceMatrix(_2D_vertex_vec);

        int slant_count = countSlant(block_distance_matrix);
        if (slant_count < (u + 1) * slant_threshold_ratio)
            return _2D_vertex_vec;
    }

    return std::vector<std::vector<vertex>>(0);
}

std::vector<std::vector<vertex>> globalDivide(std::vector<vertex> vertex_vec, unsigned u) {
    std::vector<std::vector<vertex>> _2D_vertex_vec;

    float size = vertex_vec.size();
    int index = 2;
    float threshold_index = 0.0f;

    std::vector<vertex> temp_vec1;
    temp_vec1.emplace_back(vertex_vec[1]);
    _2D_vertex_vec.emplace_back(temp_vec1);
    while ((index + size / u) < vertex_vec.size()) {
        std::vector<vertex> temp_vec2;
        threshold_index += size / u;
        for (index; index < threshold_index; index++)
            temp_vec2.emplace_back(vertex_vec[index]);

        if (!temp_vec2.empty())    
            _2D_vertex_vec.emplace_back(temp_vec2);
    }

    std::vector<vertex> temp_vec3;
    for (index; index < size; index++)
        temp_vec3.emplace_back(vertex_vec[index]);

    if (!temp_vec3.empty())
        _2D_vertex_vec.emplace_back(temp_vec3);

    return _2D_vertex_vec;
}

dpAns HeldKarp(std::vector<std::vector<vertex>>& _2D_vertex_vec, std::vector<vertex> vertex_vec) {
    unsigned size = _2D_vertex_vec.size() - 1;
    //printf("DEBUG(HeldKarp - HeldKarp) size is %d\n", size);

    std::vector<std::vector<double>> block_distance_matrix = buildBlockDistanceMatrix(_2D_vertex_vec);
    
    double prev_distance = 0.0f;
    for (unsigned i = 0; i < size; i++)
        prev_distance += block_distance_matrix[i][i + 1];
    prev_distance += block_distance_matrix[size][0];

    /*
    printf("DEBUG(HeldKarp - HeldKarp) block distance matrix\n");
    for (unsigned i = 0; i < size + 1; i++) {
        std::cout << i << ": ";
        for (unsigned j = 0; j < size + 1; j++) {
            printf("[%f] ", block_distance_matrix[i][j]);
        }
        std::cout << std::endl;
    }
    */

    std::vector<std::vector<float>> distance_matrix;
    std::vector<std::vector<unsigned>> prev_point_matrix;

    for (unsigned i = 0; i < size + 1; i++) {
        std::vector<float> temp_vec(1 << size, FLT_MAX);
        std::vector<unsigned> temp_unsigned_vec(1 << size, -1);
        temp_vec[0] = pointDistance(vertex_vec[1].getPoint(), _2D_vertex_vec[i].begin()->getPoint());
        distance_matrix.emplace_back(temp_vec);
        prev_point_matrix.emplace_back(temp_unsigned_vec);
    }

    for (unsigned i = 1; i < size + 1; i++) {
        std::vector<unsigned> index_vec = produceCombination(size, i);

        for (unsigned j = 0; j < index_vec.size(); j++) {
            unsigned index = index_vec[j];
            for (unsigned l = 1; l < size + 1; l++) {
                if (isInclude(index, l)) {
                    distPoint distPoint_temp = find_minimum(_2D_vertex_vec, vertex_vec, distance_matrix, prev_point_matrix, block_distance_matrix, l, index);
                    distance_matrix[l][index] = distPoint_temp.distance;
                    prev_point_matrix[l][index] = distPoint_temp.point_index;
                }
            }
        }
    }

    unsigned fullS = (1 << size) - 1;
    distPoint distPoint_temp = find_minimum(_2D_vertex_vec, vertex_vec, distance_matrix, prev_point_matrix, block_distance_matrix, 0, fullS);
    distance_matrix[0][fullS] = distPoint_temp.distance;
    prev_point_matrix[0][fullS] = distPoint_temp.point_index;

    //printf("DEBUG(HeldKarp - HeldKarp) distance matrix[0][fullS] = %f\n", distance_matrix[0][fullS]);

    /*
    printf("DEBUG (HeldKarp - HeldKarp) prev point matrix\n\n");
    for (unsigned i = 0; i < prev_point_matrix.size(); i++) {
        std::cout << i << ": ";
        for (unsigned j = 0; j < prev_point_matrix[i].size(); j++)
            printf("[%d ] ", prev_point_matrix[i][j]);
        
        std::cout << std::endl;
    }
    */

    std::vector<unsigned> new_permutation = extractPermutation(prev_point_matrix);
    
    /*
    printf("DEBUG (HeldKarp - HeldKarp) permutation\n");
    for(unsigned i = 0; i < new_permutation.size(); i++)
        printf("[%5d] ", new_permutation[i]);
    
    std::cout << std::endl;
    */

    //printf("DEBUG (HeldKarp - HeldKarp) new [%f] prev [%f]\n", distPoint_temp.distance, prev_distance);

    return {distPoint_temp.distance < prev_distance * 0.99, false, distPoint_temp.distance, new_permutation};
}

// simple Held-Karp Alg for very low n < 14
void HeldKarpTSP(std::vector<vertex> positions, int dimension) {
    std::vector<std::vector<vertex>> vertex_position;
    std::vector<vertex> verVec;
    dpAns dpAns_min;
    dpAns_min.change = true;
    double start_distance = 0.0f;
    for(unsigned i = 1; i < positions.size() - 1; i++)
        start_distance += pointDistance(positions[i].getPoint(), positions[i + 1].getPoint());
    start_distance += pointDistance(positions[1].getPoint(), positions[positions.size() - 1].getPoint());
    dpAns_min.distance = start_distance;

    dpAns_min.permutation = range(dimension - 1);
    dpAns_min.permutation.emplace_back(0);

    int u = 13; // Held-Karp [1962]
    vertex_position = localDivide(positions, u, 2);

    dpAns dpAns_temp = HeldKarp(vertex_position, positions);

    if(dpAns_temp.change)
        dpAns_min = dpAns_temp;

    for (unsigned i = 0; i < dpAns_min.permutation.size() - 1; i++) {
        printf("[%5d] ", positions[dpAns_min.permutation[i] + 1].getIndex());
        if((i + 1) % 8 == 0)
            std::cout << std::endl;
    }

    std::cout << "\nTotal Distance: " << dpAns_min.distance << std::endl;
}

void appHeldKarpTSP(std::vector<vertex> positions, int dimension) {
    /*
    // preprocessing for CLRS 2-approximation
    std::vector<unsigned> CLRS_permutation = getCLRS_TSP(positions, dimension);
    std::vector<vertex> temp_positions(1, vertex({0, 0}, 0));
    for(unsigned i = 0; i < CLRS_permutation.size(); i++)
        temp_positions.emplace_back(positions[CLRS_permutation[i]]);
    
    positions = temp_positions;
    */

    std::vector<std::vector<vertex>> vertex_position;
    std::vector<vertex> verVec;
    dpAns dpAns_min;
    dpAns_min.change = true;
    double start_distance = 0.0f;
    for(unsigned i = 1; i < positions.size() - 1; i++)
        start_distance += pointDistance(positions[i].getPoint(), positions[i + 1].getPoint());
    start_distance += pointDistance(positions[1].getPoint(), positions[positions.size() - 1].getPoint());
    dpAns_min.distance = start_distance;

    dpAns_min.permutation = range(dimension - 1);
    dpAns_min.permutation.emplace_back(0);

    //printf("DEBUG (HeldKarp - app) start distance %f\n", dpAns_min.distance);

    bool local_turn = true;
    int fail_count = 0;
    int u = 13; // Held-Karp [1962]
    double slant_threshold_ratio = 0.5;

    while (true) {
        /*
        for (unsigned i = 0; i < positions.size(); i++) {
            point tempP = positions[i].getPoint();
            printf("%d : [%d] %f %f\n", i, positions[i].getIndex(), tempP.x, tempP.y);
        }
        */
        if(local_turn) {
            //printf("local mode\n");
            vertex_position = localDivide(positions, u, slant_threshold_ratio);
            if (vertex_position.empty()) {
                local_turn = !local_turn;
                slant_threshold_ratio = 3;
                continue;
            }
        }
        else {
            //printf("global mode\n");
            vertex_position = globalDivide(positions, u);
        }

        /*
        printf("DEBUG (HeldKarp - appHeldKarp) vertex position size is %d\n", vertex_position.size());
        
        for (unsigned i = 0; i < vertex_position.size(); i++) {
            std::cout << i << " : ";
            for (unsigned j = 0; j < vertex_position[i].size(); j++)
                printf("[%d ] ", vertex_position[i][j].getIndex());

            std::cout << std::endl;
        }
        */

        dpAns dpAns_temp = HeldKarp(vertex_position, positions);
        //printf("DEBUG (HeldKarp - app) new answer is %f and %d\n", dpAns_temp.distance, dpAns_temp.change);
        //printf("DEBUG(HeldKarp - app) dpAns min has %d, %f\n", dpAns_min.change, dpAns_min.distance);

        if (!dpAns_temp.change) {
            fail_count++;

            if (fail_count >= 5)
                break;

            if (local_turn)
                slant_threshold_ratio -= 0.1;
        } else {
            fail_count = 0;
            dpAns_min = dpAns_temp;

            positions = reshapeVertexVector(positions[1], vertex_position, dpAns_min.permutation);
            
            if (!local_turn) {
                local_turn = true;
                slant_threshold_ratio = 0.5;
            }
        }
        /*
        printf("DEBUG(HeldKarp - app) permutation\n");
        for (unsigned i = 0; i < dpAns_temp.permutation.size(); i++) {
            printf("[%d ] ", dpAns_temp.permutation[i]);
        }
        std::cout << std::endl;
        */
        /*
        printf("DEBUG(HeldKarp - app) reshape Vector\n");
        for(unsigned i = 1; i < positions.size(); i++) {
            printf("[%5d] ", positions[i].getIndex());
            if (i % 8 == 0)
                std::cout << std::endl;
        }
        std::cout << std::endl;
        */
    }

    printf("DEBUG (HeldKarp - approximation) size is %d\n", positions.size());

    for (unsigned i = 1; i < positions.size(); i++) {
        printf("[%5d] ", positions[i].getIndex());
        if((i) % 8 == 0)
            std::cout << std::endl;
    }

    printf("\nTotal Distance: %f\n", dpAns_min.distance);
}   

std::vector<vertex> reshapeVertexVector(vertex start_vertex, std::vector<std::vector<vertex>> _2D_vertex_vec, std::vector<unsigned> permutation) {
    std::vector<vertex> temp_vec;
    point zero = {0, 0};
    temp_vec.emplace_back(vertex(zero, 0));

    for(unsigned i = 0; i < permutation.size() - 1; i++) {
        unsigned index = permutation[i];
        for(unsigned j = 0; j < _2D_vertex_vec[index].size(); j++)
            temp_vec.emplace_back(_2D_vertex_vec[index][j]);
    }

    return temp_vec;
}

// DEBUG
void printMatrix(std::vector<std::vector<vertex>> _2D_vertex_vec) {
    for (unsigned i = 0; i < _2D_vertex_vec.size(); i++) {
        for (unsigned j = 0; j < _2D_vertex_vec[i].size(); j++) {
            point tempP = _2D_vertex_vec[i][j].getPoint();
            printf("[%d] %f %f\n", _2D_vertex_vec[i][j].getIndex(), tempP.x, tempP.y);
        }
    }
}