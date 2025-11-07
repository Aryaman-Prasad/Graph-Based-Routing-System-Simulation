#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

Node* nearest_node(Graph &G, std::pair<double, double> &p, std::string &poi){
    double min_dist = INF;
    Node* node = nullptr;

    for (int i=0; i<G.V; i++){
        if (poi == "any" || G.vertices[i]->check_poi(poi)){
            double d = G.distance(p, G.vertices[i]);

            if (d < min_dist){
                min_dist = d;
                node = G.vertices[i];
            }
        }
    }

    return node;
}

// MINOR Changes and formatting too be  made
std::vector<std::pair<Node*, double>> KNN_sssp(Graph &G, Node* s, int k, std::map<Node*, Node*> &parent, std::string &poi){ // Implementation of sp and parent is flexible, kept as map for clarity
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;
    std::map<Node*, double> sp;

    // Initialization...
    // int n = G.V;

    // sp.assign(n, INF);
    // parentassign(n, nullptr);
    // Can be needed if sp and parent are defined as vectors...

    std::vector<std::pair<Node*, double>> KNN;
    std::map<Node*, bool> M;

    sp[s] = 0.0;
    unknown.push({s, 0.0});
    M[s] = true;

    int count = 0;

    // Dijkstra...
    while (!unknown.empty() && count < k){
        std::pair<Node*, double> v = unknown.top();
        unknown.pop();

        // if(v.second == INF){
        //     break;
        // }

        if (!M[v.first]){
            count++;
            KNN.push_back(v);
            M[v.first] = true;
        }

        if (v.second != sp[v.first]){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){ // Now its corret :D
            if (e->get_dest()->check_poi(poi) && (sp.find(e->get_dest()) == sp.end() || sp[v.first] + e->get_length() < sp[e->get_dest()])){

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