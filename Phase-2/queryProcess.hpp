#ifndef QUERY_PROCESS_HPP
#define QUERY_PROCESS_HPP

#include "common.hpp"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

json process_query(json query, Graph &G){
    json result;

    // Query for K shortest paths (exact), idk time complexity
    if (query["type"] == "k_shortest_paths"){
        
        // Shortest paths w.r.t distance
        if (query["mode"] == "distance"){
            int s = query["source"];
            int e = query["target"];
            int k = query["k"];

            Node* start = G.vertices[s];
            Node* dest = G.vertices[e];

            std::vector<Path> A = KSP(G, start, dest, k);

            std::vector<json> paths;

            for (Path p : A){
                json temp;
                temp["length"] = p.length;
                std::vector<int> nodes;

                for (auto n : p.vertices){
                    nodes.push_back(n->getid());
                }

                temp["path"] = nodes;
                paths.push_back(temp);
            }

            result["id"] = query["id"];
            result["paths"] = paths;
            return result;
        }
    }

    // Query for approx. shortest path
    else if (query["type"] == "approx_shortest_path"){
        result["id"] = query["id"];
        std::vector<json> distances;

        for (json i : query["queries"]){
            Node* s = G.vertices[i["source"]];
            Node* t = G.vertices[i["target"]];

            double d = a_sharp(G, s, t);

            json distance;
            distance["source"] = i["source"];
            distance["target"] = i["target"];
            distance["approx_shortest_distance"] = d;

            distances.push_back(distance);
        }

        result["distances"] = distances;
        return result;
    }
    
    // No other possible input should exist
    assert(false);
}

#endif