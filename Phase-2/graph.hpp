#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <cmath>
#include <limits>
#include <algorithm>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Header file for graph, can additionally make something like a graphUtils.cpp or 
// graphUtils.hpp for implementations any functions defined here

class Node{
    // Node attributes (id, latitude, longitude etc)...
    int id;
    double lat,lon;
    bool restricted;
    std::vector<std::string> pois;

public:
    // Constructor...
    Node(int id, double lat, double lon, std::vector<std::string> pois) : id(id), lat(lat), lon(lon), pois(pois), restricted(false){}

    // Destructor...
    ~Node(){}

    // Relevant functions if needed...
    bool isRestricted();
    int getid();
    double get_lat();
    double get_lon();
    void updateRestriction(bool d);
    bool check_poi(std::string poi);
};

class Edge{
    // Edge attributes (id, length, speed_profile, oneway etc)...
    int id;
    double length;
    Node* dest;
    double avg_time;
    std::vector<double> speed_profile;
    bool oneway;
    bool restricted;
    std::string road_type;

public:
    // Constructor...
    Edge(int id, double length, Node* dest, double avg, std::vector<double> speed_profile, bool oneway, std::string road_type) : id(id), length(length), dest(dest), avg_time(avg), speed_profile(speed_profile), oneway(oneway), restricted(false), road_type(road_type){}

    // Destructor...
    ~Edge(){}

    // Functions to call for getting certain Edge attributes (can make the variables directly public but this looks cooler hehe)
    int getId();
    Node* get_dest();
    double get_length();
    std::string getType();
    double getTime();
    std::vector<double> getProfile();
    void update_length(double len);
    void update_avg_time(double time);
    bool isRestricted();
    bool isOneway();
    void updateRestriction(bool d);
    void updateProfile(std::vector<double> newProfile);
    void updateType(std::string newType);

    // Relevant functions if needed...

};

class Graph{
public:
    std::vector<std::vector<Edge*>> adj; // It is now a vector :D
    std::vector<Node*> vertices;
    std::map<int, std::pair<int, int>> edges; // Map to store edge id and their corresponding vertex id's that are connected by it
    int V; // Number of vertices
    int E; // Number of edges, if required

    // Constructor...
    Graph(int v) : V(v), E(0) {
        // Reserve space in the vectors...
        adj.reserve(v);
        vertices.reserve(v);
    }

    // Destructor...
    ~Graph(){}

    // Adding a Node
    void addNode(Node* v);

    // Removing a Node, can implement as complete delete or mark Node to not be used
    void removeNode(Node* v);

    // Adding an Edge, input source Node and Edge (which consists of destination Node)
    void addEdge(Node* v, Edge* e, bool d);

    // Removing an Edge, implementation for this too is flexible, here edge_id is passed in the query...
    bool removeEdge(int edge_id);

    // Modifying / restoring the edge
    bool modifyEdge(int edge_id, json &patch);

    // Other relevant functions, whatever required...
    Node* getNode(int id);

    // Euclidian distance between two nodes (different from edge length)
    double distance(std::pair<double, double> v1,Node* v2);

};

struct Path{
    std::vector<int> vertices;
    double length;

    bool operator<(const Path& other) const{
        if (length == other.length) {
            return vertices.size() < other.vertices.size();
        }
        return length < other.length;
    }
    bool operator>(const Path& other) const{
        if (length == other.length) {
            return vertices.size() > other.vertices.size();
        }
        return length > other.length;
    }
};


#endif
