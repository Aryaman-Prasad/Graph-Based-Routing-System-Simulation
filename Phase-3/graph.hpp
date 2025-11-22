#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <cmath>
#include <algorithm>
#include <random>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Node{
    int id;
    double lat,lon;
    bool restricted;
    std::vector<std::string> pois;

public:
    Node(int id, double lat, double lon, std::vector<std::string> pois) : id(id), lat(lat), lon(lon), restricted(false),pois(pois){}

    ~Node(){}

    bool isRestricted();
    int getid();
    double get_lat();
    double get_lon();
    void updateRestriction(bool d);
    bool check_poi(std::string poi);
};

class Edge{
    int id;
    double length;
    Node* dest;
    double avg_time;
    std::vector<double> speed_profile;
    bool oneway;
    bool restricted;
    std::string road_type;

public:
    Edge(int id, double length, Node* dest, double avg, std::vector<double> speed_profile, bool oneway, std::string road_type) : id(id), length(length), dest(dest), avg_time(avg), speed_profile(speed_profile), oneway(oneway), restricted(false), road_type(road_type){}

    ~Edge(){}

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
};

class Graph{
public:
    std::vector<std::vector<Edge*>> adj;
    std::vector<Node*> vertices;
    std::map<int, std::pair<int, int>> edges; // Map to store edge id and their corresponding vertex id's that are connected by it
    int V; 
    int E;

    Graph(int v) : V(v), E(0) {
        adj.reserve(v);
        vertices.reserve(v);
    }

    ~Graph(){}

    void addNode(Node* v);

    void removeNode(Node* v);

    void addEdge(Node* v, Edge* e, bool d);

    bool removeEdge(int edge_id);

    bool modifyEdge(int edge_id, json &patch);

    Node* getNode(int id);

    double distance(std::pair<double, double> v1,Node* v2);

};

// Path struct for storing paths in the graph
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
