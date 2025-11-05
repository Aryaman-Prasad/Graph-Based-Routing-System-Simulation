#include "graph.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

json process_query(json query, Graph &G){
    if (query["type"] == "remove_edge"){
        bool m = G.removeEdge(query["edge_id"]);
        json result;
        result["id"] = query["id"];
        result["done"] = m;
        return result;
    }
    return query;
}