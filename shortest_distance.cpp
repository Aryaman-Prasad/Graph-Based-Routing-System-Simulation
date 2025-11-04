#include "graphUtils.cpp"

#ifndef INF
#define INF 1000000000
#endif

// Comparison operator for priority queue
struct cmp{
    bool operator()(std::pair<Node*, int> a, std::pair<Node*, int> b){
        return a.second > b.second;
    }
};

void sssp(Graph &G, Node* s, std::map<Node*, bool> &sp, std::map<Node*, Node*> &parent){ // Implementation of sp and parent is flexible, kept as map for clarity
    struct cmp;
    std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, cmp> unknown;

    // Initialization...
    int n = G.V;

    // sp.assign(n, INF);
    // parentassign(n, nullptr);
    // Can be needed if sp and parent are defined as vectors...

    sp[s] = 0;
    unknown.push({s, 0});

    // Dijkstra...
    while (!unknown.empty()){
        std::pair<Node*, int> v = unknown.top();
        unknown.pop();

        if (v.second != sp[v.first]){
            continue;
        }

        for (Edge* e : G.adj[v.first]){
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

    return;
}