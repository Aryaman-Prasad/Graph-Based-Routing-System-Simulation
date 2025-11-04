#include "graph.hpp"

#ifndef INF
#define INF 1000000000
#endif

// Comparison operator for priority queue
struct cmp{
    bool operator()(std::pair<Node*, int> &a, std::pair<Node*, int> &b){
        return a.second > b.second;
    }
};
// MINOR Changes and formatting too be  made
std::vector<std::pair<Node*,int>> KNN_sssp(Graph &G, Node* s, int k, std::map<Node*, Node*> &parent){ // Implementation of sp and parent is flexible, kept as map for clarity
    std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, cmp> unknown;
    std::map<Node*, int> sp;
    // Initialization...
    int n = G.V;

    // sp.assign(n, INF);
    // parentassign(n, nullptr);
    // Can be needed if sp and parent are defined as vectors...
    std::vector<std::pair<Node*,int>> KNN;
    std::map<Node*,bool> M;

    sp[s] = 0;
    unknown.push({s, 0});
    M[s]=1;

    int count=0;
    // Dijkstra...
    while (!unknown.empty() && count<k){
        std::pair<Node*, int> v = unknown.top();
        unknown.pop();

        if(v.second==INF)break;

        if(!M[v.first]){
            count++;
            KNN.push_back(v);
            M[v.first]=1;
        }

        if (v.second != sp[v.first]){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){ // THIS NEEDS TO BE CHANGED NOW THAT adj IS A VECTOR INSTEAD OF MAP
            if (sp[v.first] + e->get_length() < sp[e->get_dest()]){
                // Updating shortest path
                sp[e->get_dest()] = sp[v.first] + e->get_length();

                // Updating parent, may be required for other purposes
                parent[e->get_dest()] = v.first;

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({e->get_dest(), sp[e->get_dest()]});
            }
        }
    }

    return KNN;
}
