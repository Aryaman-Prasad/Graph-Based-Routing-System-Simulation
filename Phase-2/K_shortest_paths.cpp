#include <./graph.hpp>
#include <./shortest_distance.cpp>
#include <./P_path.hpp>

std::vector<path> KSP(Graph &G, Node* start, Node* dest, int k){
    std::vector<path> A;
    std::vector<path> B;
    std::map<path,bool> M;

    std::map<Node*, int> sp;
    std::map<Node*, Node*> parent;
    sssp(G, start, sp, parent);
    path first=P_path(start,dest, sp, parent);
    if(first.vertices.size()==0)return;

    A.push_back(first);
    M[first]=1;

    for(int i=1;i<k;i++){
        path& X_path=A[i-1];
        for(int j=0;j<(int)X_path.vertices.size()-1;j++){
            auto spurNode=A[i-1].vertices[j];
            path rootPath;
            rootPath.vertices=std::vector<Node*> (A[i-1].vertices.begin(),A[i-1].vertices.begin()+j+1);
            std::vector<Edge*> restricted_edges;
            std::vector<Node*> restricted_nodes;
            for(auto &p:A){
                if(p.vertices.size()>j && rootPath.vertices==std::vector<Node*> (p.vertices.begin(),p.vertices.begin()+j+1)){
                    Node* v1=p.vertices[j];
                    Node* v2=p.vertices[j+1];
                    for(auto &edge:G.adj[v1->getid()]){
                        if(edge->get_dest()->getid()==v2->getid()){
                            if(!edge->isRestricted())restricted_edges.push_back(edge);
                            edge->updateRestriction(true);
                        }
                    }
                }
            }
            for(auto node:rootPath.vertices){
                if(node!=spurNode){
                    if(!node->isRestricted())restricted_nodes.push_back(node);
                    node->updateRestriction(true);
                }
            }

            std::map<Node*, int> sp2;
            std::map<Node*, Node*> parent2;
            sssp(G, spurNode, sp2, parent2);
            path spurPath =P_path(spurNode, dest, sp2,parent2);
            if(spurPath.vertices.size()==0){
                for(auto &x:restricted_edges){
                    x->updateRestriction(false);
                }
                for(auto &x:restricted_nodes){
                    x->updateRestriction(false);
                }
                continue;
            }
            path total_path;
            total_path.vertices=rootPath.vertices;
            total_path.vertices.insert(total_path.vertices.end(),spurPath.vertices.begin()+1,spurPath.vertices.end());
            total_path.length=rootPath.length+spurPath.length;
            if(M.find(total_path)==M.end()){
                B.push_back(total_path);
                M[total_path]=1;
            }
            for(auto &x:restricted_edges){
                x->updateRestriction(false);
            }
            for(auto &x:restricted_nodes){
                x->updateRestriction(false);
            }

        }
        if(B.size()==0){
            break;
        }
        std::sort(B.begin(),B.end());
        A.push_back(B.front());
        B.erase(B.begin());
    }
    return B;
}
