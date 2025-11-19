#include "common.hpp"


std::vector<Path> KSP(Graph &G, Node* start, Node* dest, int &k){
    std::vector<Path> A;
    std::priority_queue<Path, std::vector<Path>, Pathcmp> B;
    std::unordered_set<std::string> M;
    M.reserve(2*k);
    std::vector<double> sp(G.V, INF);
    std::vector<int> parent(G.V, -1);

    sssp(G, start, dest->getid(), sp, parent);

    Path first = P_path(start, dest, sp, parent);

    if (first.vertices.empty()){
        return {};
    }

    std::vector<std::unordered_map<int,double>> edge_lengths(G.V);
    for(int i=0; i<G.V; i++){
        for(Edge* e:G.adj[i]){
            edge_lengths[i][e->get_dest()->getid()] = e->get_length();
        }
    }

    A.push_back(first);
    M.insert(path_string(first.vertices));

    for(int i=1; i<k; i++){
        Path& X_path = A[i-1];

        for(int j=0; j<(int) X_path.vertices.size() - 1; j++){
            auto spurNode = A[i-1].vertices[j];
            Path rootPath;
            rootPath.vertices.reserve(j+1);
            rootPath.vertices.assign(A[i-1].vertices.begin(), A[i-1].vertices.begin() + j + 1);

            rootPath.length = 0.0;

            for (size_t t = 0; t + 1 < rootPath.vertices.size(); ++t) {
                int u = rootPath.vertices[t];
                int v = rootPath.vertices[t + 1];
                rootPath.length += edge_lengths[u][v];
            }

            std::vector<Edge*> restricted_edges;
            std::vector<Node*> restricted_nodes;

            for (auto &p : A){
                if (p.vertices.size() > j && std::equal(rootPath.vertices.begin(), rootPath.vertices.end(), p.vertices.begin())){
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

            std::vector<double> sp2(G.V, INF);
            std::vector<int> parent2(G.V, -1);

            sssp(G, G.vertices[spurNode], dest->getid(), sp2, parent2);

            Path spurPath = P_path(G.vertices[spurNode], dest, sp2, parent2);

            if (spurPath.vertices.empty()){
                for(auto &x : restricted_edges){
                    x->updateRestriction(false);
                }

                for (auto &x : restricted_nodes){
                    x->updateRestriction(false);
                }

                continue;
            }

            Path total_path;
            total_path.vertices.reserve(rootPath.vertices.size() + spurPath.vertices.size());
            total_path.vertices = rootPath.vertices;
            total_path.vertices.insert(total_path.vertices.end(), spurPath.vertices.begin() + 1, spurPath.vertices.end());
            total_path.length = rootPath.length + spurPath.length;

            std::string find_mp=path_string(total_path.vertices);
            if (M.find(find_mp) == M.end()){
                B.push(std::move(total_path));
                M.insert(std::move(find_mp));
            }

            for(auto &x : restricted_edges){
                x->updateRestriction(false);
            }

            for(auto &x : restricted_nodes){
                x->updateRestriction(false);
            }

        }

        if (B.empty()){
            break;
        }

        A.push_back(B.top());
        B.pop();
    }

    return A;
}