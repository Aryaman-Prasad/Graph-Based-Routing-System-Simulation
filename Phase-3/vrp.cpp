#include <random>
#include "graph.hpp"
#include "ls.cpp"

#ifndef INF
#define INF 1000000000
#endif

Solution getInitialSolution(Graph& G, int m, std::vector<Order> orders, Node* depot) {
    Solution s;
    s.orders = std::move(orders);

    // Initialize m empty routes
    for (int k = 0; k < m; k++) {
        Path* p = new Path();
        p->vertices.push_back(depot);
        p->vertices.push_back(depot);
        s.routes.push_back(p);
    }
    // Register mapping
    for (int idx = 0; idx < s.orders.size(); idx++) {
        s.pickup_to_order[G.getNode(s.orders[idx].pickup)] = s.orders[idx].id;
        s.delivery_to_order[G.getNode(s.orders[idx].delivery)] = s.orders[idx].id;
    }
    // CHEAPEST INSERTION for each order
    for (auto& ord : s.orders) {
        Node* P = G.getNode(ord.pickup);
        Node* D = G.getNode(ord.delivery);
        double best_inc = INF;
        int best_r = -1, best_i = -1, best_j = -1;
        // Try inserting into each route
        for (int r = 0; r < s.routes.size(); r++) {
            auto& R = s.routes[r]->vertices;
            for (int i = 1; i < (int)R.size(); i++) {          // pickup pos
                for (int j = i; j < (int)R.size(); j++) {      // delivery pos
                    // Temporary insertion
                    R.insert(R.begin() + i, P);
                    R.insert(R.begin() + j + 1, D);
                    if (isRouteFeasible(G, R, s)) {
                        double newCost = s.getCost(G);
                        // Undo to get original
                        R.erase(R.begin() + j + 1);
                        R.erase(R.begin() + i);
                        double oldCost = s.cost;
                        double inc = newCost - oldCost;
                        if (inc < best_inc) {
                            best_inc = inc;
                            best_r = r;
                            best_i = i;
                            best_j = j;
                        }
                    } else {
                        // Undo
                        R.erase(R.begin() + j + 1);
                        R.erase(R.begin() + i);
                    }
                }
            }
        }
        // *** Perform the best insertion found ***
        auto& Rbest = s.routes[best_r]->vertices;
        Rbest.insert(Rbest.begin() + best_i, P);
        Rbest.insert(Rbest.begin() + best_j + 1, D);
        s.cost = s.getCost(G);
    }
    return s;
}

void RVND(Solution& s, Graph& G) {
    std::vector<int> NL = {1,2,3,4,5};    // neighborhood ids
    std::mt19937 rng(std::random_device{}());
    std::shuffle(NL.begin(), NL.end(), rng);

    while (!NL.empty()) {
        int nh = NL.back(); NL.pop_back();
        bool improved = false;

        if (nh == 1) {improved = IntraRouteSwap(s, G); break;}
        else if (nh == 2) {improved = IntraRouteReinsertion(s, G); break;}
        else if (nh == 3) {improved = IntraRoute2Opt(s, G); break;}
        else if (nh==4) {improved = InterRouteSwap(s, G); break;}
        else if (nh==5) {improved = InterRouteReinsertion(s, G); break;}

        if (improved) {
            NL = {1,2,3,4,5};
            std::shuffle(NL.begin(), NL.end(), rng);
        }
    }
}

void perturb(Graph& G, Solution& s) {
    // ===== PARAMETERS =====
    int k = 3;  // number of orders to remove
    if (s.orders.size() <= k) return;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> distOrder(0, s.orders.size() - 1);

    // ===== HELPER: relatedness metric =====
    auto relatedness = [&](const Order& a, const Order& b) {
        Node* ap = G.getNode(a.pickup);
        Node* bp = G.getNode(b.pickup);
        Node* ad = G.getNode(a.delivery);
        Node* bd = G.getNode(b.delivery);

        double dp = G.distance({ap->get_lat(), ap->get_lon()}, bp);
        double dd = G.distance({ad->get_lat(), ad->get_lon()}, bd);

        return dp + dd;
    };

    // ===== 1. SELECT REMOVAL SEED ORDER =====
    int seed_idx = distOrder(rng);
    Order seed = s.orders[seed_idx];

    std::vector<int> removedIds;         // store indices in s.orders
    removedIds.push_back(seed_idx);

    // ===== 2. REMOVE K RELATED ORDERS =====
    for (int r = 1; r < k; r++) {
        double best_rel = 1e18;
        int best_idx = -1;

        for (int i = 0; i < s.orders.size(); i++) {
            if (std::find(removedIds.begin(), removedIds.end(), i) != removedIds.end())
                continue;

            double rel = relatedness(seed, s.orders[i]);
            if (rel < best_rel) {
                best_rel = rel;
                best_idx = i;
            }
        }
        removedIds.push_back(best_idx);
    }

    // ===== 3. REMOVE THESE ORDERS FROM ALL ROUTES =====
    std::vector<Order> removedOrders;
    for (int idx : removedIds) removedOrders.push_back(s.orders[idx]);

    for (auto& pathPtr : s.routes) {
        auto& R = pathPtr->vertices;

        R.erase(std::remove_if(R.begin(), R.end(), [&](Node* u) {
            for (auto& o : removedOrders)
                if (u->getid() == o.pickup || u->getid() == o.delivery)
                    return true;
            return false;
        }), R.end());
    }

    s.cost = s.getCost(G);

    // ===== 4. GREEDY REINSERTION =====
    for (auto& ord : removedOrders) {
        Node* P = G.getNode(ord.pickup);
        Node* D = G.getNode(ord.delivery);
        double best_inc = INF;
        int best_r = -1, best_i = -1, best_j = -1;
        // Try inserting into every route
        for (int r = 0; r < s.routes.size(); r++) {
            auto& R = s.routes[r]->vertices;
            for (int i = 1; i <= (int)R.size(); i++) {     // pickup
                for (int j = i; j <= (int)R.size(); j++) { // delivery
                    // Perform temporary insertion
                    R.insert(R.begin() + i, P);
                    R.insert(R.begin() + j + 1, D);
                    if (isRouteFeasible(G, R, s)) {
                        double newC = s.getCost(G);
                        double oldC = s.cost;
                        double inc = newC - oldC;
                        if (inc < best_inc) {
                            best_inc = inc;
                            best_r = r;
                            best_i = i;
                            best_j = j;
                        }
                    }
                    // Undo
                    R.erase(R.begin() + j + 1);
                    R.erase(R.begin() + i);
                }
            }
        }
        // ===== Perform best known placement =====
        auto& Rbest = s.routes[best_r]->vertices;
        Rbest.insert(Rbest.begin() + best_i, P);
        Rbest.insert(Rbest.begin() + best_j + 1, D);
        s.cost = s.getCost(G);
    }
}

//Don't transfer Solution objects around, maybe use pointers instead
Solution vrpspd(Graph& G, int m, int max_iter, std::vector<Order> orders, Node* depot) {
    Solution best = getInitialSolution(G, m, orders, depot);
    RVND(best,G);

    for (int iter = 0; iter < max_iter; iter++) {
        Solution s = best;
        perturb(G,s);
        RVND(s,G);
        if (s.getCost(G) < best.getCost(G)) {
            best = s;
            iter = 0;
        }
    }
    return best;
}