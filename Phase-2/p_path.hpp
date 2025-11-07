#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

Path P_path(Node* start, Node* dest, std::map<Node*, double>& sp, std::map<Node*, Node*>& parent) {
    Path path;

    if (sp.find(dest) == sp.end() || sp[dest] == INF)
        return path; // no path

    Node* curr = dest;

    while (curr != nullptr) {
        path.vertices.push_back(curr);
        curr = parent[curr];
        if (curr==start) {
            path.vertices.push_back(start);
            break;
        }
    }

    std::reverse(path.vertices.begin(), path.vertices.end());
    path.length = sp[dest];
    return path;
}