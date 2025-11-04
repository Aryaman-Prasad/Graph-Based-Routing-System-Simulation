#include "graph.hpp"

// NODE Class Functions //
bool Node::isRestricted(){
    return restricted;
}

int Node::getid(){
    return id;
}

double Node::get_lat(){
    return lat;
}

double Node::get_lon(){
    return lon;
}

// Implementation of functions for Node, Edge and Graph classes

void Graph::addNode(Node* v){
    V++;
}

void Graph::removeNode(Node* v){
    // Completely removing the node, this can be changed...
    adj.erase(v); // THIS NEEDS TO BE CHANGED NOW THAT adj IS A VECTOR INSTEAD OF MAP
    V--;
}

void Graph::addEdge(Node* v, Edge* e){
    adj[v->getid()].push_back(e); // Is this enough? Should we push back the edge into adjacency list of destination as well?
    E++;
}

void Graph::removeEdge(Node* v, Edge* e){
    // Can implement as just marking Edge to not be used, might be better...

}

Node* Graph::getNode(int id){
    return vertices[id];
}

double Graph::distance(Node* v1,Node* v2){
    double dx=v1->get_lat()-v2->get_lat();
    double dy=v1->get_lon()-v2->get_lon();
    return sqrt(dx*dx + dy*dy);
}
// EDGE Class Functions //
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

// Other relevant functions
