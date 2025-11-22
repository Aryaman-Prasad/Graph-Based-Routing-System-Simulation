#include "common.hpp"


std::vector<int> KNN_euclidean(Graph &G, std::pair<double, double> &s, int &k, std::string &poi){
    
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp_r> unknown;
    int n = G.V;

    for(int i=0; i<n; i++){
        // We consider only if it is same poi we need
        if (G.vertices[i]->check_poi(poi)){
            unknown.push({G.vertices[i], G.distance(s, G.vertices[i])});
        }

        // Here we maintain only K shortest elements
        if ((int) unknown.size() > k){
            unknown.pop();
        }
    }

    std::vector<int> KNN;

    while (!unknown.empty()){
        KNN.push_back(unknown.top().first->getid());
        unknown.pop();
    }
    // As it from unknown we get the order of nodes in reverse order
    std::reverse(KNN.begin(),KNN.end());

    return KNN;
}