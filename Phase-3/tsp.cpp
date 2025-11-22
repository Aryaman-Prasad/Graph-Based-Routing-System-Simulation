#include "common.hpp"

void scheduling(const Graph &G, std::vector<Order> &orders, std::vector<Boy> &boys, std::unordered_map<int, DijkstraResult> &D, int num_b, int num_o){
    int remaining = num_o;

    while (remaining > 0){
        double bestCompletion = INF;
        int bestBoy = -1;
        int bestOrder = -1;
        double bestD1 = 0.0, bestD2 = 0.0;

        for (int b=0; b<num_b; b++){
            int pos = boys[b].current_pos;
            if (!D.count(pos)){
                continue;
            }
            auto &Db = D[pos].sp;
            for (int i=0; i<num_o; i++){
                if (orders[i].state != NOT_ASSIGNED){
                    continue;
                }
                int pu = orders[i].pickup;
                int de = orders[i].delivery;
                double d1 = Db[pu];
                if (d1 >= INF/2){
                    continue;
                }
                if (!D.count(pu)){
                    continue;
                }
                double d2 = D[pu].sp[de];
                if (d2 >= INF/2){
                    continue;
                }
                double completion = boys[b].current_time + d1 + d2;
                if (completion < bestCompletion){
                    bestCompletion = completion;
                    bestBoy = b;
                    bestOrder = i;
                    bestD1 = d1;
                    bestD2 = d2;
                }
            }
        }

        if (bestBoy == -1){
            break;
        }

        int b = bestBoy;
        int i = bestOrder;
        int pu = orders[i].pickup;
        int de = orders[i].delivery;

        std::vector<int> path1 = reconstruct_path(boys[b].current_pos, pu, D[boys[b].current_pos].parent);
        if (path1.empty()){ // Unexpected
            orders[i].state = DELIVERED;
            orders[i].delivery_time = INF;
            orders[i].owner = b;
            remaining--;
            continue;
        }

        for (auto idx = 1; idx < path1.size(); idx++){
            boys[b].full_path.push_back(path1[idx]);
        }
        boys[b].current_time += bestD1;
        boys[b].current_pos = pu;

        std::vector<int> path2 = reconstruct_path(pu, de, D[pu].parent);
        if (path2.empty()) {
            orders[i].state = DELIVERED;
            orders[i].delivery_time = INF;
            orders[i].owner = b;
            remaining--;
            continue;
        }

        for (auto idx = 1; idx < path2.size(); idx++){
            boys[b].full_path.push_back(path2[idx]);
        }
        boys[b].current_time += bestD2;
        boys[b].current_pos = de;

        orders[i].state = ASSIGNED;
        orders[i].owner = b;
        orders[i].delivery_time = bestCompletion;
        boys[b].handled_orders.push_back(orders[i].id);

        remaining--;
    }
}

double assignment(const Graph &G, std::vector<Order> &orders, std::vector<Boy> &boys, int num_b, int num_o){
    std::vector<std::vector<std::pair<int, double>>> visits(num_b);
    for (int b=0; b<num_b; b++){
        double t = 0.0;
        int cur = boys[b].full_path.front();
        visits[b].push_back({cur, t});
        for (auto idx = 1; idx<boys[b].full_path.size(); idx++){
            int nxt = boys[b].full_path[idx];
            double w = get_edge_weight(G, cur, nxt);
            if (w >= INF/2){
                // Since paths are computed from shortest paths, this shouldn't happen
                w = 0;
            }
            t += w;
            visits[b].push_back({nxt, t});
            cur = nxt;
        }

        if (!visits[b].empty()){
            boys[b].current_time = visits[b].back().second;
        }
        else{
            boys[b].current_time = 0.0;
        }
    }

    for (int i=0; i<num_o; ++i) {
        orders[i].state = NOT_ASSIGNED;
        orders[i].owner = -1;
        orders[i].delivery_time = -1.0;
    }

    for (int i=0; i<num_o; i++){
        int pu = orders[i].pickup;
        int de = orders[i].delivery;
        double best_delivery_time = INF;
        int best_boy = -1;

        for (int b=0; b<num_b; b++){
            double t_pick = -1.0;
            int idx_pick = -1;
            for (auto idx = 0; idx<visits[b].size(); idx++){
                if (visits[b][idx].first == pu){
                    t_pick = visits[b][idx].second;
                    idx_pick = (int) idx;
                    break;
                }
            }
            if (idx_pick == -1){
                continue; // Never visits pickup
            }

            double t_del = -1.0;
            int idx_del = -1;
            for (auto j = idx_pick + 1; j < visits[b].size(); j++){
                if (visits[b][j].first == de) {
                    t_del = visits[b][j].second;
                    idx_del = (int)j;
                    break;
                }
            }
            if (idx_del == -1){
                continue; // Never gets delivered after pickup
            }

            if (t_del < best_delivery_time - 1e-9 || (fabs(t_del - best_delivery_time) < 1e-9 && b < best_boy)) {
                best_delivery_time = t_del;
                best_boy = b;
            }
        }

        if (best_boy != -1){
            orders[i].state = DELIVERED;
            orders[i].owner = best_boy;
            orders[i].delivery_time = best_delivery_time;
        }
        else{
            orders[i].state = NOT_ASSIGNED; // unreachable via routes
        }
    }

    std::vector<std::vector<std::pair<double, int>>> perBoyDelivered(num_b);
    for (int i = 0; i < num_o; i++) {
        if (orders[i].state == DELIVERED && orders[i].owner >= 0) {
            perBoyDelivered[orders[i].owner].push_back({orders[i].delivery_time, orders[i].id});
        }
    }
    for (int b = 0; b < num_b; b++) {
        sort(perBoyDelivered[b].begin(), perBoyDelivered[b].end());
        boys[b].handled_orders.clear();
        for (auto &p : perBoyDelivered[b]) {
            boys[b].handled_orders.push_back(p.second);
        }
    }

    double sum_times = 0.0;
    int undel = 0;
    for (int i=0; i<num_o; i++){
        if (orders[i].state == DELIVERED && orders[i].delivery_time < INF/2){
            sum_times += orders[i].delivery_time;
        }
        else {
            undel++;
        }
    }

    return sum_times;
}