#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

Path P_path(Node* start, Node* dest, std::vector<double>& sp, std::vector<int>& parent) {
    Path path;

    if (sp.size()<dest->getid() || sp[dest->getid()] == INF)
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

std::string path_string(const std::vector<int>& vertices){
    std::string s;
    for(size_t i=0;i<vertices.size();i++){
        if(i)s+=',';
        s+=std::to_string(vertices[i]);
    }
    return s;
}
