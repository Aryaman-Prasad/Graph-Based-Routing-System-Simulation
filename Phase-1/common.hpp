// Common header file for inclusion of shortest_distance.cpp, 
// shortest_time.cpp, knn_shortest_path.cpp and knn_shortest_euclidean.cpp.
// Can also include all remaining headers here if needed...

#include "graph.hpp"

struct cmp{
    bool operator()(std::pair<Node*, int>& a, std::pair<Node*, int>& b){
        return a.second > b.second;
    }
};

struct cmp_d{
    bool operator()(std::pair<Node*, double>& a, std::pair<Node*, double>& b){
        return a.second > b.second;
    }
};

void sssp(Graph &G, Node* s, std::map<Node*, int> &sp, std::map<Node*, Node*> &parent, std::map<std::string, bool> &forbidden_roads);

double get_travel_time(Edge* e, double arrival_time);
void shortest_time(Graph &G, Node* s, std::map<Node*, double> &arrival_time, std::map<Node*, Node*> &parent, std::map<std::string, bool> &forbidden_roads);

std::vector<std::pair<Node*,int>> KNN_sssp(Graph &G, Node* s, int k, std::map<Node*, Node*> &parent);

std::vector<std::pair<Node*,double>> KNN_euclidean(Graph &G, Node* s, int k);