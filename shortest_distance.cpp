#include "./graph.hpp"

struct cmp{
    bool operator()(std::pair<Node*, int> a, std::pair<Node*, int> b){
        return a.second > b.second;
    }
};

void sssp(Graph &G, Node* s, std::map<Node*, bool> &visited){ // Implementation of visited is flexible, kept as map for clarity
    
    std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, cmp> unknown;
    std::map<Node*, int> sp; // This can be vector as well, kept as map for clarity

    // Initialization...
    for (auto v : visited){
        if (v.first == s) unknown.push({v.first, 0}); // This won't work unless we change condition or write an operator overload
        else unknown.push({v.first, __INT32_MAX__});
    }
    sp[s] = 0;

    // Dijkstra...
    while (!unknown.empty()){
        std::pair<Node*, int> v = unknown.top();

        for (Edge* e : G.adj[v.first]){
            if (!visited[e->get_dist()]){
                // Decrease priority of destination Node (idk how, need to think)...


                sp[e->get_dist()] = (sp[e->get_dist()] < sp[v.first] + e->get_length()) ? sp[e->get_dist()] : sp[v.first] + e->get_length();
            }
        }

        visited[v.first] = true;
    }

    return;
}