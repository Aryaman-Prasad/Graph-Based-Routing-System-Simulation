#include "./graph.hpp"
#include "./heap.hpp"

void sssp(Graph &G, Node* s, std::map<Node*, bool> &visited){ // Implementation of visited is flexible, kept as map for clarity
    std::map<Node*,int> val;
    Heap unknown(val);
    std::map<Node*, int> sp;// This can be vector as well, kept as map for clarity
    // Initialization...
    for (auto v : visited){
        val[v.first]=__INT32_MAX__;
        unknown.insert(v.first);
    }
    sp[s] = 0;
    unknown.decreasepriority(s);
    // Dijkstra...
    while (unknown.size()>0){
        auto v = unknown.min();
        unknown.deleteMin();
        for (Edge* e : G.adj[v]){
            if (!visited[e->get_dist()]){
                // Decrease priority of destination Node (idk how, need to think)...
                val[e->get_dist()]=sp[v] + e->get_length();
                unknown.decreasepriority(e->get_dist());
                sp[e->get_dist()] = (sp[e->get_dist()] < sp[v] + e->get_length()) ? sp[e->get_dist()] : sp[v] + e->get_length();
            }
        }

        visited[v] = true;
    }

    return;
}