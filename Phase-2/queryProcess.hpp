#ifndef QUERY_PROCESS_HPP
#define QUERY_PROCESS_HPP

#include "common.hpp"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

json process_query(json query,Graph &G, std::vector<std::vector<double>> &ref_sssp){
    json result;

    try{
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

                for (Path &p : A){
                    json temp;
                    temp["length"] = p.length;
                    temp["path"] = p.vertices;
                    paths.push_back(temp);
                }

                result["id"] = query["id"];
                result["paths"] = paths;
                return result;
            }
        }

        // Query for K shortest path (heuristic), idk time complexity
        else if (query["type"] == "k_shortest_paths_heuristic"){
            int s = query["source"];
            int e = query["target"];
            int k = query["k"];
            double t = ((double) query["overlap_threshold"]) / 100;

            Node* start = G.vertices[s];
            Node* end = G.vertices[e];

            std::vector<Path> lowest = KSP_heuristic(G, start, end, k, t);

            std::vector<json> paths;

            for (Path &p : lowest){
                json path;
                path["length"] = p.length;
                path["path"] = p.vertices;

                paths.push_back(path);
            }

            result["id"] = query["id"];
            result["paths"] = paths;
            return result;
        }

        // Query for approx. shortest path, idk time complexity
        else if (query["type"] == "approx_shortest_path"){
            // result["id"] = query["id"];
            // std::vector<json> distances;

            // for (json i : query["queries"]){
            //     Node* s = G.vertices[i["source"]];
            //     Node* t = G.vertices[i["target"]];

            //     double d = a_sharp(G, s, t);

            //     json distance;
            //     distance["source"] = i["source"];
            //     distance["target"] = i["target"];
            //     distance["approx_shortest_distance"] = d;

            //     distances.push_back(distance);
            // }

            // result["distances"] = distances;
            // return result;

            result["id"] = query["id"];
            std::vector<json> distances;

            for (json i : query["queries"]){
                double d = approx_dist(ref_sssp, G.vertices[i["source"]], G.vertices[i["target"]]);

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
        throw("Incorrect query");
    }
    catch (char const* &e){
        result.clear();
        result["error"] = e;
        return result;
    }
    return result;
}

#endif