#ifndef QUERY_PROCESS_HPP
#define QUERY_PROCESS_HPP

#include "common.hpp"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

json process_query(json query, Graph &G){
    json result;

    try{
        std::vector<json> orders = query["orders"];
        int depot = query["fleet"]["depot_node"];
        std::vector<Order> os;
        for (json o : orders){
            Order order;
            order.id = o["order_id"];
            order.pickup = o["pickup"];
            order.delivery = o["dropoff"];
            order.owner = -1;
            order.delivery_time = -1.0;
            order.state = NOT_ASSIGNED;
            os.push_back(order);
        }
        int m = query["fleet"]["num_delivery_guys"];
        int num = os.size();
        std::vector<Boy> boys(m);
        initialize(boys, m, depot);

        std::unordered_map<int, DijkstraResult> D = precompute(G, os, num, depot);

        scheduling(G, os, boys, D, m, num);

        double cost = assignment(G, os, boys, m, num);

        std::vector<json> as;
        for (int b=0; b<m; b++){
            json a;
            a["driver_id"] = b;
            a["route"] = boys[b].full_path;
            a["order_ids"] = boys[b].handled_orders;
            as.push_back(a);
        }

        result["assignments"] = as;
        result["metrics"]["total_delivery_time_s"] = cost;

        return result;

    }
    catch (char const* &e){
        result.clear();
        result["error"] = e;
        return result;
    }
}

#endif