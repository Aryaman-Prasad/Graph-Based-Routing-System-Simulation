#include <iostream>
#include <vector>
#include <map>
#include <queue>
// Header file for graph, can additionally make something like a graphUtils.cpp or 
// graphUtils.hpp for implementations any functions defined here

class Node{
    // Node attributes (id, latitude, longitude etc)...
    int id;
    double lat,lon;
    bool restricted;
public:
    // Constructor...
    Node(int id,double lat,double lon,bool restricted):id(id),lat(lat),lon(lon),restricted(restricted){}

    // Destructor...
    ~Node(){}
    bool isRestricted();
    // Relevant functions if needed...

};

class Edge{
    // Edge attributes (id, length, speed_profile, oneway etc)...
    int id;
    int length;
    Node* dest;
    std::vector<double> speed_profile;
    bool restricted;
    std::string road_type;
public:
    // Constructor...
    Edge(int id,int length,Node* dest,std::vector<double> speed_profile,bool restricted,std::string road_type):id(id),length(length),dest(dest),speed_profile(speed_profile),restricted(restricted),road_type(road_type){}

    // Destructor...
    ~Edge(){}

    // Functions to call for getting certain Edge attributes (can make the variables directly public but this looks cooler hehe)
    Node* get_dest();
    int get_length();
    void update_length(int len);
    bool isRestricted();
    // Relevant functions if needed...

};

class Graph{
public:
    std::map<Node*, std::vector<Edge*>> adj; // Can be vector (hash) instead of map, I have written map for clarity
    int V; // Number of vertices
    int E; // Number of edges, if required

    // Constructor...
    Graph():V(0),E(0){}

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

