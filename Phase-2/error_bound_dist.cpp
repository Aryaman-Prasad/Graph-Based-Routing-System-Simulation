#include "common.hpp"

#ifndef INF
#define INF std::numeric_limits<double>::max()
#endif

void sssp_all(Graph &G, Node* s, std::vector<double> &sp){
    std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> unknown;

    // If source node is restricted then even god does not know what to do...
    if (s->isRestricted()){
        return ;
    }

    sp[s->getid()] = 0.0;
    unknown.push({0.0, s});

    // Dijkstra...
    while (!unknown.empty()){
        std::pair<double, Node*> v = unknown.top();
        unknown.pop();

        if (v.first == INF){
            return ;
        }

        if (v.first != sp[v.second->getid()]){
            continue;
        }

        for (Edge* e : G.adj[v.second->getid()]){ // Noe it will work :D
            if (e->get_dest()->isRestricted() || e->isRestricted()){
                continue; // Skip if either the Edge or the destination Node is restricted...
            }
            
            if (sp[v.second->getid()] + e->get_length() < sp[e->get_dest()->getid()]){
                // Updating shortest path
                sp[e->get_dest()->getid()] = sp[v.second->getid()] + e->get_length();

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({ sp[e->get_dest()->getid()], e->get_dest()});
            }
        }
    }

    return ;
}

void error_bound_dist(Graph &G, double &e, std::vector<std::vector<double>> &ref_sssp){
    int r = (G.V) * (1 - e) * (1 - e);

    ref_sssp.assign(r, std::vector<double> (G.V, INF));

    for (int i=0; i<r; i++){
        sssp_all(G, G.vertices[i], ref_sssp[i]);
    }

    return ;
}

double approx_dist(std::vector<std::vector<double>> &ref_sssp, Node* a, Node* b){
    double minmax = INF;
    double maxmin = 0;

    for (auto &sp : ref_sssp){
        if (sp[a->getid()] + sp[b->getid()] < minmax){
            minmax = sp[a->getid()] + sp[b->getid()];
        }
        if (abs(sp[a->getid()] - sp[b->getid()]) > maxmin){
            maxmin = abs(sp[a->getid()] + sp[b->getid()]);
        }
    }

    return minmax;

}
