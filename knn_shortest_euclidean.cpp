#include "graph.hpp"

#ifndef INF
#define INF 1000000000
#endif

struct cmp{
    bool operator()(std::pair<Node*, double>& a, std::pair<Node*, double>& b){
        return a.second > b.second;
    }
};

std::vector<std::pair<Node*,double>> KNN_euclidean(Graph &G, Node* s, int k){
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;
    int n=G.V;

    for(int i=0;i<n;i++){
        if(G.vertices[i]!=s){
            unknown.push({G.vertices[i],G.distance(s,G.vertices[i])});
        }

        if(unknown.size()>k){
            unknown.pop();
        }
    }

    std::vector<std::pair<Node*,double>> KNN;

    while(!unknown.empty()){
        KNN.push_back(unknown.top());
        unknown.pop();
    }
    std::reverse(KNN.begin(),KNN.end());
    return KNN;
}