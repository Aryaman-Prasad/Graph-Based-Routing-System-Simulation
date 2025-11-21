#include "graph.hpp"
#include "p_path.hpp"
#include "ls.cpp"

#ifndef INF
#define INF 1000000000
#endif

Solution getInitialSolution(Graph& G, int n, std::map<int,int> checkpoints) {
    Solution s;
    //TODO: Find best set of routes greedily
    return s;
}

void RVND(Solution& s, Graph& G) {
    std::vector<int> neighborhoods = {1,2,3,4,5}; // Swap, Reinsertion, 2-opt
    bool improved = true;

    while (improved) {
        std::shuffle(neighborhoods.begin(), neighborhoods.end(), std::mt19937{std::random_device{}()});
        improved = false;
        for (int nh : neighborhoods) {
            bool ok = false;
            if (nh == 1) {ok = IntraRouteSwap(s, G);}
            else if (nh == 2) {ok = IntraRouteReinsertion(s, G);}
            else if (nh == 3) {ok = IntraRoute2Opt(s, G);}
            else if (nh==4) {ok = InterRouteSwap(s, G);}
            else if (nh==5) {ok = IntraRouteReinsertion(s, G);}

            if (ok) {
                improved = true;
                break;
            }
        }
    }
}

void perturb(Graph& G, Solution& s, int n) {
    //TODO: Fill this perturbation function
}

//Don't transfer Solution objects around, maybe use pointers instead
Solution vrpspd(Graph& G, int n, int max_iter, std::map<int,int> checkpoints) {
    Solution best = getInitialSolution(G, n, checkpoints);
    RVND(best,G);

    for (int iter = 0; iter < max_iter; iter++) {
        Solution s = best;
        perturb(G,s,n);
        RVND(s,G);
        if (s.getCost(G) < best.getCost(G)) {
            best = s;
            iter = 0;
        }
    }

    return best;
}