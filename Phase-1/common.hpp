// Common header file for inclusion of shortest_distance.cpp, 
// shortest_time.cpp, knn_shortest_path.cpp and knn_shortest_euclidean.cpp

#include "graph.hpp"

#ifndef INF
#define INF std::numeric_limits<double>::max()
#endif

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

// Single Source shortest path(Dijkstra)
void sssp(Graph &G, Node* s, int &target, std::vector<double> &sp, std::vector<int> &parent, std::map<std::string, bool> &forbidden_roads);

// To get Travel time while travelling along the edge based on the arrival time
double get_travel_time(Edge* e, double arrival_time);

// To get path which we will reach in least time
void shortest_time(Graph &G, Node* s, int &target, std::vector<double> &arrival_time, std::vector<int> &parent, std::map<std::string, bool> &forbidden_roads);

// To get the nearest node of a point in plane based on euclidean distance
Node* nearest_node(Graph &G, std::pair<double, double> &p, std::string &poi);

// To get K nearest neighbour with the given poi based on shortest path distance
std::vector<int> KNN_sssp(Graph &G, Node* s, int k, std::string &poi);

// To get K nearest neighbour with the given poi based on shortest euclidean distance
std::vector<int> KNN_euclidean(Graph &G, std::pair<double, double> &s, int &k, std::string &poi);