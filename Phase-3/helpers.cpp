#include "common.hpp"

DijkstraResult sssp_all(const Graph &G, Node* s){
    std::vector<double> sp(G.V, INF);
    std::vector<int> parent(G.V, -1);
    std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> unknown;

    if (s->isRestricted()){
        return {sp, parent};
    }
    sp[s->getid()] = 0.0;
    unknown.push({0.0, s});

    // Dijkstra...
    while (!unknown.empty()){
        std::pair<double, Node*> v = unknown.top();
        unknown.pop();
        if (v.first == INF){
            return {sp, parent};
        }
        if (v.first != sp[v.second->getid()]){
            continue;
        }
        for (Edge* e : G.adj[v.second->getid()]){
            if (e->get_dest()->isRestricted() || e->isRestricted()){
                continue;
            }
            if (sp[v.second->getid()] + e->get_length() < sp[e->get_dest()->getid()]){
                // Updating shortest path
                sp[e->get_dest()->getid()] = sp[v.second->getid()] + e->get_length();
                parent[e->get_dest()->getid()] = v.second->getid();
                unknown.push({sp[e->get_dest()->getid()], e->get_dest()});
            }
        }
    }
    return {sp, parent};
}

std::vector<int> reconstruct_path(int src, int dest, const std::vector<int> &parent){
    std::vector<int> path;
    if (src == dest){
        path.push_back(src);
        return path;
    }
    if (parent.empty()){
        return path;
    }
    int i = dest;
    while (i != -1){
        path.push_back(i);
        if (i == src){
            break;
        }
        i = parent[i];
    }
    std::reverse(path.begin(), path.end());
    if (path.empty() || path.front() != src){
        return {};
    }
    return path;
}

double get_edge_weight(const Graph &G, int u, int v){
    for (auto &e : G.adj[u]){
        if (e->get_dest()->getid() == v){
            return e->get_length();
        }
    }
    return INF;
}

void initialize(std::vector<Boy> &boys, int num_b, int depot){
    for (int b=0; b<num_b; b++){
        boys[b].id = b;
        boys[b].current_pos = depot;
        boys[b].current_time = 0.0;
        boys[b].full_path.clear();
        boys[b].full_path.push_back(depot);
        boys[b].handled_orders.clear();
    }
}
