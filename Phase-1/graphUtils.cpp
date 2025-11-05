#include "graph.hpp"

// NODE Class Functions //
bool Node::isRestricted(){
    return restricted;
}

void Node::updateRestriction(bool d){
    restricted = d;
}

// EDGE Class Functions //
int Edge::getId(){
    return id;
}

Node* Edge::get_dest(){
    return dest;
}

int Edge::get_length(){
    return length;
}

void Edge::update_length(int len){
    length = len;
}

bool Edge::isRestricted(){
    return restricted;
}

bool Edge::isOneway(){
    return oneway;
}

void Edge::updateRestriction(bool d){
    restricted = d;
}

// Implementation of functions for Node, Edge and Graph classes

void Graph::addNode(Node* v){
    // Adding nodes initially during graph construction
    vertices[v->getid()] = v;
    V++;
}

void Graph::removeNode(Node* v){
    // Marking vector to be unused since node can be restored in later queries
    vertices[v->getid()]->updateRestriction(false);
}

void Graph::addEdge(Node* v, Edge* e, bool d){
    adj[v->getid()].push_back(e);
    if (d) {
        edges[e->getId()] = {v->getid(), e->get_dest()->getid()}; // In case of undirected edges, addEdge() would be called twice but we want addition into map only once, hence the boolean variable...
    }
    E++;
}

bool Graph::removeEdge(int edge_id){
    std::pair<int, int> vers = edges[edge_id]; // Find the pair of vertices whose edge needs to be marked
    bool m = false; // Marker to check if the edge actually got removed or not

    // Iterating over edges w.r.t first node
    for (Edge* e : adj[vers.first]){
        if (e->getId() == edge_id){
            e->updateRestriction(false); // Mark edge
            m = true;
            break;
        }
    }

    // Iterating over edges w.r.t second node, if edge is oneway then no match might be found but that is fine
    for (Edge* e : adj[vers.second]){
        if (e->getId() == edge_id){
            e->updateRestriction(false); // Mark edge
            m = true;
            break;
        }
    }

    return m;
}


// Other relevant functions
