#include "common.hpp"

std::unordered_map<int, DijkstraResult> precompute(const Graph &G, const std::vector<Order> &orders, int num_o, int depot){
    // Select important nodes
    std::vector<int> imp_nodes;
    imp_nodes.push_back(depot);
    for (int i=0; i<num_o; i++){
        imp_nodes.push_back(orders[i].pickup);
        imp_nodes.push_back(orders[i].delivery);
    }
    std::sort(imp_nodes.begin(), imp_nodes.end());
    imp_nodes.erase(unique(imp_nodes.begin(), imp_nodes.end()), imp_nodes.end());

    // Precompute Dijkstra for important nodes
    std::unordered_map<int, DijkstraResult> D;
    D.reserve(imp_nodes.size()*2);
    for (int node : imp_nodes){
        D[node] = sssp_all(G, G.vertices[node]);
    }

    return D;
}