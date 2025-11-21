#include "graph.hpp"

#ifndef INF
#define INF 1000000000
#endif

class Solution {
    std::vector<std::vector<Edge*>> routes;
public:
    double getCost() {
        //TODO: Devise a cost function for each solution
    }
};

class LocalSearch {
public:
    LocalSearch(){}

    void IntraRouteSwap(Solution& s);
    void IntraRoute2Opt(Solution& s);
    void IntraRouteReinsertion(Solution& s);

    void InterRouteSwap(Solution& s);
    void InterRouteReinsertion(Solution& s);

private:
    bool applySwap(Solution& s);          // returns true if solution improved
    bool apply2Opt(Solution& s);
    bool applyReinsertion(Solution& s);

    // helpers
    void updateCost(Solution& s);
};

Solution getInitialSolution(Graph& G, int n, std::map<int,int> checkpoints) {
    Solution s;
    //TODO: Find best set of routes greedily
    return s;
}

Solution RVND(Solution& s) {
    Solution best;
    //TODO: Get the best solution among all neighbourhoods
    return best;
}

void perturb(Graph& G, Solution& s, int n) {
    //TODO: Fill this perturbation function
}

//Don't transfer Solution objects around, maybe use pointers instead
Solution vrpspd(Graph& G, int n, int max_iter, std::map<int,int> checkpoints) {
    Solution best = getInitialSolution(G, n, checkpoints);
    //RVND(best);

    for (int iter = 0; iter < max_iter; iter++) {
        Solution s = best;
        perturb(G,s,n);
        //RVND(s);
        if (s.getCost() < best.getCost()) {
            best = s;
            iter = 0;
        }
    }

    return best;
}