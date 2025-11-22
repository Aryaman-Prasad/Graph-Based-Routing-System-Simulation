#include "graph.hpp"
#include "p_path.hpp"

#ifndef INF
#define INF 1000000000
#endif

struct Order {
    int pickup;
    int delivery;
    int id;
};

class Solution {
public:
    double cost = 0.0;
    std::vector<Path*> routes;
    std::unordered_map<Node*, int> pickup_to_order;  
    std::unordered_map<Node*, int> delivery_to_order;
    std::vector<Order> orders;

    double getCost(Graph& G) {
        double total = 0.0;
        for (auto& pathPtr : routes) {
            auto& route = pathPtr->vertices;
            for (int i = 0; i + 1 < (int)route.size(); i++) {
                Node* u = route[i];
                Node* v = route[i+1];
                bool found = false;
                double best = INF;
                // Find if edge exists directly
                for (Edge* e : G.adj[u->getid()]) {
                    if (e->get_dest()->getid() == v->getid()) {
                        best = std::min(best, e->getTime());
                        total += best;
                        found = true;
                    }
                }
            }
        }
        cost = total;
        return total;
    }
};

bool isRouteFeasible(Graph& G, const std::vector<Node*>& route, const Solution& s) {
    std::unordered_map<int,int> pos;
    for (int i = 0; i < (int)route.size(); i++)
        pos[route[i]->getid()] = i;

    for (auto& c : s.orders) {
        int p = c.pickup;
        int d = c.delivery;
        if (pos.count(p) && pos.count(d)) {
            if (pos[d] < pos[p]) {return false;}
        }
    }

    for (int i = 0; i+1 < (int)route.size(); i++) {
        Node* u = route[i];
        Node* v = route[i+1];
        bool edgeExists = false;
        // Look for an edge u -> v in the adjacency list
        for (Edge* e : G.adj[u->getid()]) {
            if (e->get_dest()->getid() == v->getid()) {
                edgeExists = true;
                break;
            }
        }
        if (!edgeExists) {return false;}
    }
    return true;
}

bool IntraRoute2Opt(Solution& s, Graph& G) {
    bool improved = false;
    double cost = s.getCost(G);

    for (auto& route : s.routes) {
        int n = route->vertices.size();

        for (int i = 1; i < n-2; i++) {
            for (int j = i+1; j < n-1; j++) {
                std::reverse(route->vertices.begin()+i, route->vertices.begin()+j+1);
                if (isRouteFeasible(G, route->vertices, s)) {
                    double newCost = s.getCost(G);
                    if (newCost < cost) {
                        improved = true;
                    }
                    else {std::reverse(route->vertices.begin()+i, route->vertices.begin()+j+1);}
                }
                else {std::reverse(route->vertices.begin()+i, route->vertices.begin()+j+1);}
            }
        }
    }
    return improved;
}

bool IntraRouteReinsertion(Solution& s, Graph& G) {
    bool improved = false;

    for (auto& route : s.routes) {
        int n = route->vertices.size();
        double cost = s.getCost(G);
        bool improved_now = false;

        for (int i = 1; i < n-1; i++) {
            Node* node = route->vertices[i];
            route->vertices.erase(route->vertices.begin() + i);

            for (int j = 1; j < n-1; j++) {
                route->vertices.insert(route->vertices.begin() + j, node);
                if (isRouteFeasible(G, route->vertices, s)) {
                    double newCost = s.getCost(G);
                    if (newCost < cost) {improved_now = true;}
                    else {route->vertices.erase(route->vertices.begin() + j);}
                }
                else {route->vertices.erase(route->vertices.begin() + j);}
            }
            if (!improved_now) {route->vertices.insert(route->vertices.begin() + i, node);}
        }
        improved |= improved_now;
    }
    return improved;
}

bool IntraRouteSwap(Solution& s, Graph& G) {
    bool improved = false;
    double cost = s.getCost(G);

    for (auto& route : s.routes) {
        int n = route->vertices.size();
        for (int i = 1; i < n-1; i++) {
            for (int j = i+1; j < n-1; j++) {
                std::swap(route[i], route[j]);

                if (isRouteFeasible(G, route->vertices, s)) {
                    double newCost = s.getCost(G);
                    if (newCost < cost) {improved = true;}
                    else {std::swap(route[i], route[j]);}
                }
                else {std::swap(route[i], route[j]);}
            }
        }
    }
    return improved;
}

bool InterRouteReinsertion(Solution& s, Graph& G) {
    for (int ra = 0; ra < s.routes.size(); ra++) {
        auto& routeA = s.routes[ra]->vertices;
        for (int i = 0; i < routeA.size(); i++) {
            Node* u = routeA[i];
            for (int rb = 0; rb < s.routes.size(); rb++) {
                if (rb == ra) continue;
                auto& routeB = s.routes[rb]->vertices;
                // Try inserting u into every position of route rb
                for (int j = 0; j <= routeB.size(); j++) {
                    // Transfer from ra to rb
                    routeA.erase(routeA.begin() + i);
                    routeB.insert(routeB.begin() + j, u);
                    if (isRouteFeasible(G, routeA, s) && isRouteFeasible(G, routeB, s)) {
                        s.cost = s.getCost(G);
                        return true;
                    }
                    // Undo move
                    routeB.erase(routeB.begin() + j);
                    routeA.insert(routeA.begin() + i, u);
                }
            }
        }
    }
    return false; // nothing feasible
}

bool InterRouteSwap(Solution& s, Graph& G) {
    for (int ra = 0; ra < s.routes.size(); ra++) {
        auto& routeA = s.routes[ra]->vertices;
        for (int i = 0; i < routeA.size(); i++) {
            Node* u = routeA[i];
            for (int rb = 0; rb < s.routes.size(); rb++) {
                if (rb == ra) continue;
                auto& routeB = s.routes[rb]->vertices;
                // Try inserting u into every position of route rb
                for (int j = 0; j <= routeB.size(); j++) {
                    std::swap(routeA[i],routeB[j]);
                    if (isRouteFeasible(G, routeA, s) && isRouteFeasible(G, routeB, s)) {
                        s.cost = s.getCost(G);
                        return true;
                    }
                    // Undo move
                    routeB.erase(routeB.begin() + j);
                    routeA.insert(routeA.begin() + i, u);
                }
            }
        }
    }
    return false;
}