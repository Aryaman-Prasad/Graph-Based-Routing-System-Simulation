#include <../Phase-1/graph.hpp>
#include <../Phase-1/shortest_distance.cpp>
path P_path(Node* start,Node* dest, std::map<Node*, int>& sp, std::map<Node*, Node*>& parent) {
    path path;
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
    path.length=sp[dest];
    return path;
}