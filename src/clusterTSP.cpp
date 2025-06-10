#include "clusterTSP.h"


std::vector<std::vector<vertex>> kmeansPP(std::vector<vertex>& positions, int k) {
    std::vector<point> centroids;
    std::vector<std::vector<vertex>> cluster_vec;
    cluster_vec.resize(k);
    std::vector<clusterVertex> clusterPos(1, {-1, vertex({0, 0}, 0)});

    for (unsigned i = 1; i < positions.size(); i++) {
        clusterVertex temp_CV = {-1, positions[i]};
        clusterPos.emplace_back(temp_CV);
    }

    // First centeroid
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, positions.size() - 1);
    int start_idx = dis(gen);
    
    clusterPos[start_idx].cluster_id = 0;
    centroids.emplace_back(clusterPos[start_idx].v.getPoint());

    // Select remaining k-1 centeroid
    for(unsigned i = 1; i < k; i++) {
        clusterVertex *new_centroid;
        double max_dist = DBL_MIN;

        for(unsigned j = 1; j < clusterPos.size(); j++) {
            clusterVertex temp_CV = clusterPos[j];
            if(temp_CV.cluster_id == -1) {
                double sum_dist = 0.0;
                for (unsigned k = 0; k < centroids.size(); k++) {
                    point centroid_temp = centroids[k];
                    sum_dist += pointDistance(temp_CV.v.getPoint(), centroid_temp);
                }
                
                if(sum_dist > max_dist) {
                    max_dist = sum_dist;
                    new_centroid = &temp_CV;
                }
            }
        }
        new_centroid->cluster_id = i;
        centroids.push_back(new_centroid->v.getPoint());
    }

    //printf("DEBUG (clusterTSP - kmeansPP) here?\n");

    // k-means iteration
    bool changed = true;
    //int debug_iteration = 0;
    while(changed) {
        changed = false;

        //printf("DEBUG (clusterTSP - kmeansPP) [%d] here?\n", debug_iteration++);
        //if(debug_iteration > 10000)
            //break;
        
        // assign all points to the neareast centroid cluster
        for (unsigned i = 1; i < clusterPos.size(); i++) {
            clusterVertex *p = &clusterPos[i];

            double min_dist = DBL_MAX;
            int best_cluster = -1;
            
            for(int i = 0; i < centroids.size(); i++) {
                double dist = pointDistance(p->v.getPoint(), centroids[i]);
                if(dist < min_dist) {
                    min_dist = dist;
                    best_cluster = i;
                }
            }
            
            if(p->cluster_id != best_cluster) {
                p->cluster_id = best_cluster;
                changed = true;
            }
        }

        // update centroids
        for(unsigned i = 0; i < k; i++) {
            point new_pos = {0, 0};
            int count = 0;
            
            for (unsigned j = 1; j < clusterPos.size(); j++) {
                clusterVertex temp_CV = clusterPos[j];
                if(temp_CV.cluster_id == i) {
                    point p_pos = temp_CV.v.getPoint();
                    new_pos.x += p_pos.x;
                    new_pos.y += p_pos.y;
                    count++;
                }
            }
            
            if(count > 0) {
                new_pos.x /= count;
                new_pos.y /= count;
                centroids[i] = new_pos;
            }
        }
    }

    for (unsigned i = 1; i < clusterPos.size(); i++) {
        int CV_cluster_id = clusterPos[i].cluster_id;
        cluster_vec[CV_cluster_id].emplace_back(clusterPos[i].v);
    }

    return cluster_vec;
}

int estimateK(std::vector<vertex>& positions, int maxK) {
    std::vector<double> k_cluster_distortion_vec;
    int startIdx = rand() % positions.size();
    
    for(int k = 1; k <= maxK; k++) {
        std::vector<std::vector<vertex>> clusters = kmeansPP(positions, k);
        
        double J = calculateDistortion(clusters, positions);
        k_cluster_distortion_vec.push_back(J);
    }
    
    int bestK = 1;
    double maxCurvature = 0.0;
    
    double dy = k_cluster_distortion_vec[maxK-1] - k_cluster_distortion_vec[0];
    double dx = maxK - 1;
    
    for(int i = 0; i < maxK; i++) {
        double dist = std::abs(dy * i - dx * (k_cluster_distortion_vec[i] - k_cluster_distortion_vec[0])) 
                     / std::sqrt(dy*dy + dx*dx);
        
        if(dist > maxCurvature) {
            maxCurvature = dist;
            bestK = i + 1;
        }
    }
    
    return bestK;
}

double calculateDistortion(const std::vector<std::vector<vertex>>& clusters, const std::vector<vertex>& positions) {
    double totalDistortion = 0.0;
    
    for (unsigned i = 0; i < clusters.size(); i++) {
        std::vector<vertex> cluster = clusters[i];

        if(cluster.empty()) 
            continue;
        
        point centroid = {0, 0};
        for (unsigned j = 0; j < cluster.size(); j++) {
            point p = cluster[j].getPoint();
            centroid.x += p.x;
            centroid.y += p.y;
        }

        centroid.x /= cluster.size();
        centroid.y /= cluster.size();
        
        for (unsigned j = 0; j < cluster.size(); j++) {
            point p = cluster[j].getPoint();
            totalDistortion += std::pow(pointDistance(p, centroid), 2);
        }
    }
    
    return totalDistortion;
}

void clusterTSP(std::vector<vertex> positions, int dimension) {
    int bestK = estimateK(positions, 20);

    std::vector<std::vector<vertex>> cluster_vec = kmeansPP(positions, bestK);

    for (unsigned i = 0; i < cluster_vec.size(); i++) {
        std::vector<vertex> temp_vec(1, vertex());
        for (unsigned j = 0 ; j < cluster_vec[i].size(); j++)
            temp_vec.emplace_back(cluster_vec[i][j]);
        
        std::vector<unsigned> permutation = getCLRS_TSP(temp_vec, cluster_vec[i].size());
        
        cluster_vec[i].clear();
        for (unsigned j = 0; j < permutation.size(); j++) {
            cluster_vec[i].emplace_back(temp_vec[permutation[j]]);
        }
    }

    dpAns dp_answer = HeldKarp(cluster_vec, positions);

    std::vector<vertex> answer_vertex_vec;
    for(unsigned j = 0; j < dp_answer.permutation.size(); j++) {
        for (unsigned k = 0; k < cluster_vec[dp_answer.permutation[j]].size(); k++) {
            answer_vertex_vec.emplace_back(cluster_vec[dp_answer.permutation[j]][k]);
        }
    }

    for (unsigned i = 0; i < answer_vertex_vec.size() - 1; i++) {
        printf("[%5d] ", answer_vertex_vec[i].getIndex());
        if((i + 1) % 8 == 0)
            std::cout << std::endl;
    }

    std::cout << "\nTotal Distance: " << dp_answer.distance << std::endl;

    //printCluster(cluster_vec);
}

// For DEBUGu
void printCluster(std::vector<std::vector<vertex>>& _2D_vertex_vec) {
    for (unsigned i = 0; i < _2D_vertex_vec.size(); i++) {
        std::cout << i << " : ";
        for (unsigned j = 0; j < _2D_vertex_vec[i].size(); j++)
            printf("[%4d] ", _2D_vertex_vec[i][j].getIndex());
        
        std::cout << std::endl;
    }
}