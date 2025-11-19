#include "common.hpp"

Path P_path(Node* start, Node* dest, std::vector<double>& sp, std::vector<int>& parent) {
    Path path;

    if (sp.size() <= dest->getid() || sp[dest->getid()] == INF)
        return path; 

    int curr = dest->getid();

    while (true) {
        path.vertices.push_back(curr);
        curr = parent[curr];
        if (curr==start->getid()) {
            path.vertices.push_back(start->getid());
            break;
        }
    }

    std::reverse(path.vertices.begin(), path.vertices.end());
    path.length = sp[dest->getid()];
    return path;
}