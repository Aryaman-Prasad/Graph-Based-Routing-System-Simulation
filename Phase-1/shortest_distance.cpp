#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

// Comparison operator for priority queue


void sssp(Graph &G, Node* s, std::vector<double> &sp, std::vector<int> &parent, std::map<std::string, bool> &forbidden_roads){ // Implementation of sp and parent is flexible, kept as unordered_map for clarity
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;

    // If source node is restricted then even god does not know what to do...
    if (s->isRestricted()){
        return ;
    }

    // Initialization...
    int n = G.V;

    sp.assign(n, INF);
    parent.assign(n, -1);
    // Now they are vectors :D

    sp[s->getid()] = 0.0;
    unknown.push({s, 0.0});

    // Dijkstra...
    while (!unknown.empty()){
        std::pair<Node*, double> v = unknown.top();
        unknown.pop();

        if (v.second != sp[v.first->getid()]){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){ // Noe it will work :D
            if (e->get_dest()->isRestricted() || e->isRestricted() || forbidden_roads[e->getType()]){
                continue; // Skip if either the Edge or the destination Node is restricted...
            }
            
            if (sp[v.first->getid()] + e->get_length() < sp[e->get_dest()->getid()]){
                // Updating shortest path
                sp[e->get_dest()->getid()] = sp[v.first->getid()] + e->get_length();

                // Updating parent, may be required for other purposes
                parent[e->get_dest()->getid()] = v.first->getid();

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({e->get_dest(), sp[e->get_dest()->getid()]});
            }
        }
    }

    return ;
}