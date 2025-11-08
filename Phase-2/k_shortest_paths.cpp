#include "p_path.hpp"

struct Pathcmp{
    bool operator()(const Path &a, const Path &b) const {
        return a.length > b.length; 
    }
};


std::vector<Path> KSP(Graph &G, Node* start, Node* dest, int &k){
    std::vector<Path> A;
    std::priority_queue<Path,std::vector<Path>,Pathcmp> B;
    std::map<Path, bool> M;

    std::vector<double> sp;
    std::vector<int> parent;

    sssp(G, start, dest->getid(),sp, parent);

    Path first = P_path(start, dest, sp, parent);
    if (first.vertices.empty()){
        return {};
    }

    std::vector<std::unordered_map<int,double>> edge_lengths(G.V);
    for(int i=0;i<G.V;i++){
        for(Edge* e:G.adj[i]){
            edge_lengths[i][e->get_dest()->getid()]=e->get_length();
        }
    }

    A.push_back(first);
    M[first] = 1;

    for(int i=1; i<k; i++){
        Path& X_path = A[i-1];

        for(int j=0; j<(int) X_path.vertices.size() - 1; j++){
            auto spurNode = A[i-1].vertices[j];
            Path rootPath;
            rootPath.vertices = std::vector<int> (A[i-1].vertices.begin(), A[i-1].vertices.begin() + j + 1);

            rootPath.length = 0.0;

            for (size_t t = 0; t + 1 < rootPath.vertices.size(); ++t) {
                int u = rootPath.vertices[t];
                int v = rootPath.vertices[t + 1];
                rootPath.length+=edge_lengths[u][v];
            }

            std::vector<Edge*> restricted_edges;
            std::vector<Node*> restricted_nodes;

            for (auto &p : A){
                if (p.vertices.size() > j && rootPath.vertices == std::vector<int> (p.vertices.begin(), p.vertices.begin() + j + 1)){
                    int v1 = p.vertices[j];
                    int v2 = p.vertices[j+1];

                    for (auto &edge : G.adj[v1]){
                        if (edge->get_dest()->getid() == v2){
                            if (!edge->isRestricted()){
                                restricted_edges.push_back(edge);
                            }

                            edge->updateRestriction(true);
                        }
                    }
                }
            }

            for (auto node : rootPath.vertices){
                if (node != spurNode){
                    if (!G.vertices[node]->isRestricted()){
                        restricted_nodes.push_back(G.vertices[node]);
                    }

                    G.vertices[node]->updateRestriction(true);
                }
            }

            std::vector<double> sp2;
            std::vector<int> parent2;

            sssp(G, G.vertices[spurNode], dest->getid(), sp2, parent2);

            Path spurPath = P_path(G.vertices[spurNode], dest, sp2,parent2);

            if (spurPath.vertices.size() == 0){
                for(auto &x : restricted_edges){
                    x->updateRestriction(false);
                }

                for (auto &x : restricted_nodes){
                    x->updateRestriction(false);
                }

                continue;
            }

            Path total_path;
            total_path.vertices = rootPath.vertices;
            total_path.vertices.insert(total_path.vertices.end(), spurPath.vertices.begin() + 1, spurPath.vertices.end());
            total_path.length = rootPath.length + spurPath.length;

            if (M.find(total_path) == M.end()){
                B.push(total_path);
                M[total_path] = 1;
            }

            for(auto &x : restricted_edges){
                x->updateRestriction(false);
            }

            for(auto &x : restricted_nodes){
                x->updateRestriction(false);
            }

        }

        if (B.size() == 0){
            break;
        }

        A.push_back(B.top());
        B.pop();
    }

    return A;
}
