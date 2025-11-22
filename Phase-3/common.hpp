#ifndef COMMON_H
#define COMMON_H

#ifndef INF
#define INF std::numeric_limits<double>::max()
#endif

#include "graph.hpp"

struct DijkstraResult {
    std::vector<double> sp;
    std::vector<int> parent;
};

enum OrderState { NOT_ASSIGNED = 0, ASSIGNED = 1, DELIVERED = 2 };

struct Order {
    int pickup;
    int delivery;
    OrderState state = NOT_ASSIGNED;
    int id;
    int owner = -1;
    double delivery_time = -1.0;
};

struct Boy {
    int id;
    int current_pos;
    double current_time;
    std::vector<int> full_path;
    std::vector<int> handled_orders; // Sequence assigned previously; we'll recompute final mapping
};

DijkstraResult sssp_all(const Graph &G, Node* s);
std::vector<int> reconstruct_path(int src, int dest, const std::vector<int> &parent);
double get_edge_weight(const Graph &G, int u, int v);
void initialize(std::vector<Boy> &boys, int num_b, int depot);

std::unordered_map<int, DijkstraResult> precompute(const Graph &G, const std::vector<Order> &orders, int num_o, int depot);

void scheduling(std::vector<Order> &orders, std::vector<Boy> &boys, std::unordered_map<int, DijkstraResult> &D, int num_b, int num_o);
double assignment(const Graph &G, std::vector<Order> &orders, std::vector<Boy> &boys, int num_b, int num_o);






// class Solution {
// public:
//     double cost = 0.0;
//     std::vector<Path*> routes;
//     std::unordered_map<Node*, int> pickup_to_order;  
//     std::unordered_map<Node*, int> delivery_to_order;
//     std::vector<Order> orders;
//     double getCost(Graph& G) {
//         double total = 0.0;
//         for (auto& pathPtr : routes) {
//             auto& route = pathPtr->vertices;
//             for (int i = 0; i + 1 < (int)route.size(); i++) {
//                 Node* u = G.vertices[route[i]];
//                 Node* v = G.vertices[route[i+1]];
//                 bool found = false;
//                 double best = INF;
//                 // Find if edge exists directly
//                 for (Edge* e : G.adj[u->getid()]) {
//                     if (e->get_dest()->getid() == v->getid()) {
//                         best = std::min(best, e->getTime());
//                         total += best;
//                         found = true;
//                     }
//                 }
//             }
//         }
//         cost = total;
//         return total;
//     }
// };

// Path P_path(Node* start, Node* dest, std::vector<double>& sp, std::vector<int>& parent);

// double routeCost(Graph& G, std::vector<int> route);
// void sssp(Graph &G, Node* s, int target, std::vector<double> &sp, std::vector<int> &parent);
// Path shortest_path(Graph& G, Node* start, Node* dest);
// std::vector<int> expandPath(Graph& G, const std::vector<int>& seq);
// bool isRouteFeasible(Graph& G, const std::vector<int>& route, const Solution& s);
// bool IntraRoute2Opt(Solution& s, Graph& G);
// bool IntraRouteReinsertion(Solution& s, Graph& G);
// bool IntraRouteSwap(Solution& s, Graph& G);
// bool InterRouteReinsertion(Solution& s, Graph& G);
// bool InterRouteSwap(Solution& s, Graph& G);

// Solution getInitialSolution(Graph& G, int m, Solution& s, Node* depot);
// void RVND(Solution& s, Graph& G);
// void perturb(Graph& G, Solution& s);
// Solution vrpspd(Graph& G, int m, int max_iter, std::vector<Order> orders, Node* depot);

#endif