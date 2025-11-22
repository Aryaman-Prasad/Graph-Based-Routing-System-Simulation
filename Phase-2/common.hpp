// Common header file for inclusion of shortest_distance.cpp, 
// shortest_time.cpp, knn_shortest_path.cpp and knn_shortest_euclidean.cpp.
// Can also include all remaining headers here if needed...

#ifndef COMMON_H
#define COMMON_H

#ifndef INF
#define INF std::numeric_limits<double>::max()
#endif

#include "graph.hpp"

struct cmp{
    bool operator()(const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) const{
        return a.second > b.second;
    }
};

struct Pathcmp{
    bool operator()(const Path &a, const Path &b) const {
        return a.length > b.length; 
    }
};

Path P_path(Node* start, Node* dest, std::vector<double>& sp, std::vector<int>& parent);

void sssp(Graph &G, Node* s, int target, std::vector<double> &sp, std::vector<int> &parent);

std::vector<Path> KSP(Graph &G, Node* start, Node* dest, int &k);

std::vector<Path> KSP_heuristic(Graph &G, Node* start, Node* dest, int &k, double &threshold, double& total_penalty);

void error_bound_dist(Graph &G, double &e, std::vector<std::vector<double>> &ref_sssp);
double approx_dist(std::vector<std::vector<double>> &ref_sssp, Node* a, Node* b);

#endif