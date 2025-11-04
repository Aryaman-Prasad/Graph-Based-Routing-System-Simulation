#include "graph.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void process_graph(json j, Graph &G){
    // Initialization...
    int v = j["meta"]["nodes"];

    // Adding vertices...
    for (int i=0; i<v; i++){
        Node* n = new Node(j["nodes"][i]["id"], j["nodes"][i]["lat"], j["nodes"][i]["lon"]); // Include whatever parameters required
        G.addNode(n);
    }

    // Adding edges...
    int e = j["edges"].size();
    for (int i=0; i<e; i++){
        // Reading id of the two nodes part of the edge
        int u = j["edges"][i]["u"];
        int v = j["edges"][i]["v"];

        // Two edges since our Edge is essentially directed
        Edge* e1 = new Edge(j["edges"][i]["id"], 
            j["edges"][i]["length"], 
            G.getNode(v), 
            j["edges"][i]["average_time"], 
            j["edges"][i]["speed_profile"], 
            j["edges"][i]["oneway"], 
            j["edges"][i]["road_type"]); // Include whatever parameters required

        Edge* e2 = new Edge(j["edges"][i]["id"], 
            j["edges"][i]["length"], 
            G.getNode(u), 
            j["edges"][i]["average_time"], 
            j["edges"][i]["speed_profile"], 
            j["edges"][i]["oneway"], 
            j["edges"][i]["road_type"]); // Include whatever paramteres required

        G.addEdge(G.getNode(u), e1);
        G.addEdge(G.getNode(v), e2);
    }

    // Returning final graph
    return ;
}