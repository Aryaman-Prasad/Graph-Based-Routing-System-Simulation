#ifndef QUERY_PROCESS_HPP
#define QUERY_PROCESS_HPP

#include "common.hpp"
#include "nlohmann/json.hpp"

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

    // Query for Dijkstra
    if (query["type"] == "shortest_path"){

        // Minimizing distance, O(ElogE) time
        if (query["mode"] == "distance"){
            std::map<Node*, int> sp;
            std::map<Node*, Node*> parent;
            Node* s = G.vertices[query["source"]];
            std::map<std::string, bool> forbidden_roads = {{"primary" , false}, {"secondary" , false}, {"tertiary" , false}, {"local" , false}, {"expressway" , false}};

            // Adding restrictions based on query
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(true);
            }
            for (std::string i : query["constraints"]["forbidden_road_types"]){
                forbidden_roads[i] = true;
            }

            sssp(G, s, sp, parent, forbidden_roads);

            // Restoring restrictions (for nodes)
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(false);
            }

            result["id"] = query["id"];
            if (query["target"] >= G.V || sp.find(G.vertices[query["target"]]) == sp.end()){
                result["possible"] = false; // No path exists
                return result;
            }

            result["possible"] = true;
            result["minimum_time/minimum_distance"] = sp[G.vertices[query["target"]]];
            
            // Formation of path vector
            std::vector<int> ans;
            Node* temp = G.vertices[query["target"]];
            ans.push_back(temp->getid());

            while (temp->getid() != query["source"]){
                temp = parent[temp];
                ans.push_back(temp->getid());
            }
            std::reverse(ans.begin(), ans.end());

            result["path"] = ans;
            return result;
        }

        // Minimizing time, O(ElogE) time I think, but will take more time than distance mode
        else if (query["mode"] == "time"){
            std::map<Node*, double> arrival_time;
            std::map<Node*, Node*> parent;
            Node* s = G.vertices[query["source"]];
            std::map<std::string, bool> forbidden_roads = {{"primary" , false}, {"secondary" , false}, {"tertiary" , false}, {"local" , false}, {"expressway" , false}};

            // Adding restrictions based on query
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(true);
            }
            for (std::string i : query["constraints"]["forbidden_road_types"]){
                forbidden_roads[i] = true;
            }

            shortest_time(G, s, arrival_time, parent, forbidden_roads);

            // Restoring restrictions (for nodes)
            for (int i : query["constraints"]["forbidden_nodes"]){
                G.vertices[i]->updateRestriction(false);
            }

            result["id"] = query["id"];
            if (query["target"] >= G.V || arrival_time.find(G.vertices[query["target"]]) == arrival_time.end()){
                result["possible"] = false; // No path exists
                return result;
            }

            result["possible"] = true;
            result["minimum_time/minimum_distance"] = arrival_time[G.vertices[query["target"]]];
            
            // Formation of path vector
            std::vector<int> ans;
            Node* temp = G.vertices[query["target"]];
            ans.push_back(temp->getid());

            while (temp->getid() != query["source"]){
                temp = parent[temp];
                ans.push_back(temp->getid());
            }
            std::reverse(ans.begin(), ans.end());

            result["path"] = ans;
            return result;
        }
    }
    return query;
}

#endif