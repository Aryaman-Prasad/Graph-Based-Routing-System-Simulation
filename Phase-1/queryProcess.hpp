#ifndef QUERY_PROCESS_HPP
#define QUERY_PROCESS_HPP

#include "common.hpp"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

json process_query(json query, Graph &G){
    json result;

    // Query to remove edge, O(E) time
    if (query["type"] == "remove_edge"){
        bool m = G.removeEdge(query["edge_id"]);
        result["id"] = query["id"];
        result["done"] = m;
        return result;
    }

    // Query to modify edge, O(E) time
    if (query["type"] == "modify_edge"){
        bool m = G.modifyEdge(query["edge_id"], query["patch"]);
        result["id"] = query["id"];
        result["done"] = m;
        return result;
    }

    // Query for shortest path
    if (query["type"] == "shortest_path"){

        // Minimizing distance, O(ElogE) time
        if (query["mode"] == "distance"){
            std::vector<double> sp(G.V, INF);
            std::vector<int> parent(G.V, -1);
            Node* s = G.vertices[query["source"]];
            std::map<std::string, bool> forbidden_roads = {{"primary" , false}, {"secondary" , false}, {"tertiary" , false}, {"local" , false}, {"expressway" , false}};

            // Adding restrictions based on query
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(true);
            }
            for (std::string i : query["constraints"]["forbidden_road_types"]){
                forbidden_roads[i] = true;
            }

            int target = query["target"];

            sssp(G, s, target, sp, parent, forbidden_roads);

            // Restoring restrictions (for nodes)
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(false);
            }

            result["id"] = query["id"];
            if (target >= G.V || sp[target] == INF){
                result["possible"] = false; // No path exists
                return result;
            }

            result["possible"] = true;
            result["minimum_time/minimum_distance"] = sp[target];
            
            // Formation of path vector
            std::vector<int> ans;
            int temp = target;
            ans.push_back(temp);

            while (temp != query["source"]){
                temp = parent[temp];
                ans.push_back(temp);
            }
            std::reverse(ans.begin(), ans.end());

            result["path"] = ans;
            return result;
        }

        // Minimizing time, O(ElogE) time I think, but will take more time than distance mode
        else if (query["mode"] == "time"){
            std::vector<double> arrival_time(G.V, INF);
            std::vector<int> parent(G.V, -1);
            Node* s = G.vertices[query["source"]];
            std::map<std::string, bool> forbidden_roads = {{"primary" , false}, {"secondary" , false}, {"tertiary" , false}, {"local" , false}, {"expressway" , false}};

            // Adding restrictions based on query
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(true);
            }
            for (std::string i : query["constraints"]["forbidden_road_types"]){
                forbidden_roads[i] = true;
            }

            int target = query["target"];

            shortest_time(G, s, target, arrival_time, parent, forbidden_roads);

            // Restoring restrictions (for nodes)
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(false);
            }

            result["id"] = query["id"];

            if (target >= G.V || arrival_time[target] == INF){
                result["possible"] = false; // No path exists
                return result;
            }

            result["possible"] = true;
            result["minimum_time/minimum_distance"] = arrival_time[target];
            
            // Formation of path vector
            std::vector<int> ans;
            int temp = target;
            ans.push_back(temp);

            while (temp != query["source"]){
                temp = parent[temp];
                ans.push_back(temp);
            }
            std::reverse(ans.begin(), ans.end());

            result["path"] = ans;
            return result;
        }
    }

    // Query for K nearest neighbours
    else if (query["type"] == "knn"){

        // KNN based on Euclidean distance, O(VlogV) time
        if (query["metric"] == "euclidean"){
            std::pair<double, double> p;
            p.first = query["query_point"]["lat"];
            p.second = query["query_point"]["lon"];

            std::string poi = query["poi"];
            int k = query["k"];

            // O(V) optimization for k = 1
            if (k == 1){
                Node* ans = nearest_node(G, p, poi);
                
                result["id"] = query["id"];
                result["nodes"] = {ans->getid()};
                return result;
            }

            std::vector<int> ans = KNN_euclidean(G, p, k, poi);

            result["id"] = query["id"];
            result["nodes"] = ans;
            return result;
        }

        // KNN based on shortest path, O(V + ElogE) time I think, though runtime depends on k as well
        else if (query["metric"] == "shortest_path"){
            std::pair<double, double> p;
            p.first = query["query_point"]["lat"];
            p.second = query["query_point"]["lon"];
            int k = query["k"];

            // Finding nearest node of given coordinate, O(V) time
            std::string poi = "any"; // Nearest node from query point can be of any poi (am I right?)
            Node* n = nearest_node(G, p, poi);

            poi = query["poi"]; // Set poi for finding KNN

            std::vector<int> ans = KNN_sssp(G, n, k, poi);

            result["id"] = query["id"];
            result["nodes"] = ans;
            return result;
        }
    }
    
    // No other possible input should exist
    assert(false);
}

#endif