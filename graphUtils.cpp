#include "graph.hpp"

// Implementation of functions for Node, Edge and Graph classes

void Graph::addNode(Node* v){
    V++;
}

void Graph::removeNode(Node* v){
    // Completely removing the node, this can be changed...
    adj.erase(v);
    V--;
}

void Graph::addEdge(Node* v, Edge* e){
    adj[v].push_back(e); // Is this enough? Should we push back the edge into adjacency list of destination as well?
    E++;
}

void Graph::removeEdge(Node* v, Edge* e){
    // Can implement as just marking Edge to not be used, might be better...

}

// Other relevant functions