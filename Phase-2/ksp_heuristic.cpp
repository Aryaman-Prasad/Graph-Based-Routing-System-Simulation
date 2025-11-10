#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

bool sim(Graph& G, Path &p1, Path &p2, double threshold) {
    int n = p1.vertices.size() <= p2.vertices.size() ? p1.vertices.size() : p2.vertices.size();
    int n1 = p1.vertices.size();
    int n2 = p2.vertices.size();
    double count = 0.0;

    if (n==0) {
        return true;
    }

    std::set<std::pair<int, int>> s;

    for (int i=0; i<n1-1; i++){
        s.insert({p1.vertices[i], p1.vertices[i+1]}); // Insert edges of p1 into the set
    }
    for (int i=0; i<n2-1; i++){
        if (s.find({p2.vertices[i], p2.vertices[i+1]}) != s.end()){
            count += 1.0;
        }
    }

    count /= n - 1;

    if (count <= threshold) {
        return true;
    }
    return false;
}

void init_Heap(Graph& G, int pos, Path& p, std::map<int, std::priority_queue<std::pair<double, Edge*>, std::vector<std::pair<double, Edge*>>>>& H) {
    int n = p.vertices.size();

    for (int i=0; i<n-1; i++) {
        Node* u = G.vertices[p.vertices[i]];
        Node* v = G.vertices[p.vertices[i+1]];

        for (Edge* e : G.adj[u->getid()]) {
            if (e->get_dest()->getid() == v->getid() && !e->isRestricted()) {
                H[pos].push({e->get_length(), e});
            }
        }
    }
}

Path shortest_path(Graph& G, Node* start, Node* dest) {
    std::vector<double> sp(G.V, INF);
    std::vector<int> parent(G.V, -1);

    sssp(G, start, dest->getid(), sp, parent);
    Path path;
    if (sp[dest->getid()]==INF) {return path;}
    path = P_path(start, dest, sp, parent);
    return path;
}

std::vector<Path> KSP_heuristic(Graph &G, Node* start, Node* dest, int &k, double &threshold, double& total_penalty) {
    std::vector<Path> lowest;
    std::vector<Edge*> DNR;
    std::map<int, std::priority_queue<std::pair<double,Edge*>,std::vector<std::pair<double,Edge*>>>> H;

    std::vector<double> sp(G.V, INF);
    std::vector<int> parent(G.V, -1);

    sssp(G, start, dest->getid(), sp, parent);

    Path first = P_path(start, dest, sp, parent);
    if (first.vertices.empty()){
        return {};
    }
    double shortest_len = sp[dest->getid()];

    lowest.push_back(first);

    //Initialize min-heap of edges of shortest path
    init_Heap(G, 0, first, H);

    while (lowest.size()<k) {
        //Choose the latest path added to lowest, and pop its smallest edge
        auto& h = H[lowest.size()-1];
        if (h.empty()) {
            break;
        }

        while (!h.empty()) {
            auto [len, e] = h.top();
            h.pop();

            //Check if the edge is forbidden
            if (e->isRestricted()) {
                continue;
            }

            //Remove the edge from the network (temporarily)
            bool found = false;
            for (auto& x : DNR) {
                if (x->getId() == e->getId()) {
                    found = true;
                    break;
                }
            }
            if (found) {continue;}
            e->updateRestriction(true);
            //Calculate shortest path without the above edge
            Path new_sp = shortest_path(G, start, dest);
            
            if (new_sp.length == 0) {
                e->updateRestriction(false);
                DNR.push_back(e);
                continue;
            }

            bool valid = true;
            for (auto& p : lowest) {
                if (!sim(G, new_sp, p, threshold)) {
                    valid = false;
                    break;
                }
            }

            //If all paths in lowest are sufficiently dissimilar from new_sp, add new_sp to lowest
            if (valid) {
                lowest.push_back(new_sp);
                init_Heap(G, lowest.size()-1, new_sp, H);
                break;
            }
        }

        //Check if heaps corresponding to all shortest paths are empty i.e. all candidate paths have been considered
        bool allempty = true;
        for (auto& [i,h] : H) {
            if (!h.empty()) {
                allempty = false;
                break;
            }
        }
        if (allempty) {
            break;
        }
    }

    //Calculate penalty
    for (int i=0; i<lowest.size(); i++) {
        int overlap = 0;
        for (int j=i; j<lowest.size(); j++) {
            if (j == i) {
                continue;
            }
            if (!sim(G, lowest[i], lowest[j], threshold)) {
                overlap++;
            }
        }

        double len = lowest[i].length;
        double dist_penalty = (len-shortest_len)/(shortest_len) + 0.1;
        total_penalty += dist_penalty * overlap;
    }

    return lowest;

}


