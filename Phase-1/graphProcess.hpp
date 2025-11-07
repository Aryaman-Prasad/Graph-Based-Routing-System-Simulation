#ifndef GRAPH_PROCESS_HPP
#define GRAPH_PROCESS_HPP

#include "graph.hpp"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

void process_graph(json j, Graph &G){
    // Initialization...
    int v = G.V;

    // Adding vertices...
    for (int i=0; i<v; i++){
        Node* n = new Node(j["nodes"][i]["id"], j["nodes"][i]["lat"], j["nodes"][i]["lon"], j["nodes"][i]["pois"]); // Include whatever parameters required
        G.addNode(n);
    }

    // Adding edges...
    int e = j["edges"].size();
    for (int i=0; i<e; i++){
        // Reading id of the two nodes part of the edge
        int u = j["edges"][i]["u"];
        int v = j["edges"][i]["v"];

        // Check if speed profile exits...
        std::vector<double> speed_profile;

        if (((json) j["edges"][i]).contains("speed_profile")){
            speed_profile = (std::vector<double>) j["edges"][i]["speed_profile"];
        }

        // Two edges since our Edge is essentially directed
        Edge* e1 = new Edge(j["edges"][i]["id"], 
            j["edges"][i]["length"], 
            G.getNode(v), 
            j["edges"][i]["average_time"], 
            speed_profile, 
            j["edges"][i]["oneway"], 
            j["edges"][i]["road_type"]); // Include whatever parameters required

        G.addEdge(G.getNode(u), e1, true);

        if (j["edges"][i]["oneway"]){
            continue; // Skip creation of second edge since edge needs to be oneway
        }

        Edge* e2 = new Edge(j["edges"][i]["id"], 
            j["edges"][i]["length"], 
            G.getNode(u), 
            j["edges"][i]["average_time"], 
            speed_profile, 
            j["edges"][i]["oneway"], 
            j["edges"][i]["road_type"]); // Include whatever paramteres required

        // Here E will get incremented by 2 overall since two directed edges are being added...
        G.addEdge(G.getNode(v), e2, false);
    }

    // Returning final graph
    return ;
}

#endif