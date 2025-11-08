#include "common.hpp"

#ifndef INF
#define INF 1000000000.0
#endif

double h(Node* s, Node* w){
    return (abs(s->get_lat() - w->get_lat()) + abs(s->get_lon() - w->get_lon()));
}

double a_sharp(Graph &G, Node* s, Node* t){ // Implementation of sp and parent is flexible, kept as map for clarity
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;
    std::map<Node*, double> sp;

    // If source node is restricted then even god does not know what to do...
    if (s->isRestricted()){
        return INF;
    }

    sp[s] = 0.0;
    unknown.push({s, 0.0});

    // A Sharp...
    while (!unknown.empty()){
        std::pair<Node*, double> v = unknown.top();
        unknown.pop();

        if (v.first->getid() == t->getid()){
            return sp[t];
        }

        if (v.second != sp[v.first] + h(s, v.first)){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){ // Noe it will work :D
            if (e->get_dest()->isRestricted() || e->isRestricted()){
                continue; // Skip if either the Edge or the destination Node is restricted...
            }
            
            if (sp.find(e->get_dest()) == sp.end() || sp[v.first] + e->get_length() < sp[e->get_dest()]){
                // Updating shortest path
                sp[e->get_dest()] = sp[v.first] + e->get_length();

                // Updating parent, may be required for other purposes
                // parent[e->get_dest()] = v.first;

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({e->get_dest(), sp[e->get_dest()] + h(s, e->get_dest())});
            }
        }
    }

    if (sp.find(t) == sp.end()){
        return INF;
    }

    return sp[t];
}