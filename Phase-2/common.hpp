// Common header file for inclusion of shortest_distance.cpp, 
// shortest_time.cpp, knn_shortest_path.cpp and knn_shortest_euclidean.cpp.
// Can also include all remaining headers here if needed...

#ifndef COMMON_H
#define COMMON_H

#include "graph.hpp"

struct cmp{
    bool operator()(const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) const{
        return a.second > b.second;
    }
};

void sssp(Graph &G, Node* s, int target, std::vector<double> &sp, std::vector<int> &parent);

std::vector<Path> KSP(Graph &G, Node* start, Node* dest, int &k);

double h(Node* s, Node* w);
double a_sharp(Graph &G, Node* s, Node* t);

#endif