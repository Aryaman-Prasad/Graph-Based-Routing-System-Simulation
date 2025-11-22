#include "common.hpp"

#ifndef INF
#define INF std::numeric_limits<double>::max()
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
std::vector<int> KNN_sssp(Graph &G, Node* s, int k, std::string &poi){ // Implementation of sp and parent is flexible, kept as map for clarity
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;
    std::vector<double> sp;

    // Initialization...
    int n = G.V;

    sp.assign(n, INF);

    std::vector<int> KNN;
    std::map<Node*, bool> M;

    sp[s->getid()] = 0.0;
    unknown.push({s, 0.0});
    // M[s] = true;

    int count = 0;

    // Dijkstra...
    while (!unknown.empty() && count < k){
        std::pair<Node*, double> v = unknown.top();
        unknown.pop();

        if (v.second == INF){
            return KNN;
        }

        if (!M[v.first] && v.first->check_poi(poi)){
            count++;
            KNN.push_back(v.first->getid());
            M[v.first] = true;
        }

        if (v.second != sp[v.first->getid()]){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){ // Now its corret :D
            if (e->isRestricted() || e->get_dest()->isRestricted()){
                continue;
            }

            if (sp[v.first->getid()] + e->get_length() < sp[e->get_dest()->getid()]){

                // Updating shortest path
                sp[e->get_dest()->getid()] = sp[v.first->getid()] + e->get_length();

                // Updating parent, may be required for other purposes
                // parent[e->get_dest()] = v.first;

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({e->get_dest(), sp[e->get_dest()->getid()]});
            }
        }
    }

    return KNN;

}
