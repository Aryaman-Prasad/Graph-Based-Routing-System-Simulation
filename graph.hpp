#include <iostream>
#include <vector>
#include <map>
#include <queue>

// Header file for graph, can additionally make something like a graphUtils.cpp or 
// graphUtils.hpp for implementations any functions defined here

class Node{
    // Node attributes (id, latitude, longitude etc)...

public:
    // Constructor...
    Node(){}

    // Destructor...
    ~Node(){}

    // Relevant functions if needed...

};

class Edge{
    // Edge attributes (id, length, speed_profile, oneway etc)...
    int length;

    // My rough idea: Edge stores the destination Node, so Graph will be defined
    // with adjacency list/map with a specific Node having multiple Edges which
    // inturn consist of the destination Node corresponding to that Edge
    Node* dest;

public:
    // Constructor...
    Edge(){}

    // Destructor...
    ~Edge(){}

    // Functions to call for getting certain Edge attributes (can make the variables directly public but this looks cooler hehe)
    Node* get_dest();
    int get_length();

    // Relevant functions if needed...

};

class Graph{
public:
    std::map<Node*, std::vector<Edge*>> adj; // Can be vector (hash) instead of map, I have written map for clarity
    int V; // Number of vertices
    int E; // Number of esges, if required

    // Constructor...
    Graph(){}

    // Destructor...
    ~Graph(){}

    // Adding a Node
    void addNode(Node* v);

    // Removing a Node, can implement as complete delete or mark Node to not be used
    void removeNode(Node* v);

    // Adding an Edge, input source Node and Edge (which consists of destination Node)
    void addEdge(Node* v, Edge* e);

    // Removing an Edge, implementation for this too is flexible, here Node (source) has to be taken as input (or maybe not necessary?)
    void removeEdge(Node* v, Edge* e);

    // Other relevant functions, whatever required...

};

Node* Edge::get_dest(){
    return dest;
}

int Edge::get_length(){
    return length;
}