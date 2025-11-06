// Common header file for inclusion of shortest_distance.cpp, 
// shortest_time.cpp, knn_shortest_path.cpp and knn_shortest_euclidean.cpp.
// Can also include all remaining headers here if needed...

#include "graph.hpp"

// Comparison operator for minheap (used for shortest distance, shortest time)
struct cmp{
    bool operator()(const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) const{
        return a.second > b.second;
    }
};

// Comparison operator for maxheap (used for KNN euclidean)
struct cmp_r{
    bool operator()(const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) const{
        return a.second < b.second;
    }
};

void sssp(Graph &G, Node* s, std::map<Node*, double> &sp, std::map<Node*, Node*> &parent, std::map<std::string, bool> &forbidden_roads);

double get_travel_time(Edge* e, double arrival_time);
void shortest_time(Graph &G, Node* s, std::map<Node*, double> &arrival_time, std::map<Node*, Node*> &parent, std::map<std::string, bool> &forbidden_roads);

Node* nearest_node(Graph &G, std::pair<double, double> &p);
std::vector<std::pair<Node*, double>> KNN_sssp(Graph &G, Node* s, int k, std::map<Node*, Node*> &parent);

std::vector<std::pair<Node*, double>> KNN_euclidean(Graph &G, std::pair<double, double> &s, int &k);