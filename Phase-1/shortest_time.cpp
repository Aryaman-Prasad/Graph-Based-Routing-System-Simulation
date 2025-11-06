#include "common.hpp"

#ifndef INF
#define INF 1000000000
#endif

#ifndef TOT_SECONDS
#define TOT_SECONDS 86400
#endif

#ifndef INTERVAL_TIME
#define INTERVAL_TIME 900
#endif

#ifndef PROFILE_LEN
#define PROFILE_LEN 96
#endif

double get_travel_time(Edge* e, double arrival_time){
    double dist_left = e->get_length();
    double time = 0;

    while (dist_left > 0){
        double speed = (e->getProfile())[((int) arrival_time / INTERVAL_TIME) % PROFILE_LEN];
        double interval_time = ((double) INTERVAL_TIME - arrival_time);
        double dist_coverable = speed * interval_time;

        if (dist_coverable >= dist_left){
            time += (dist_left) / speed;
            dist_left = 0;
            continue;
        }

        time += (dist_coverable) / speed;
        dist_left -= dist_coverable;
        arrival_time = 0;
    }

    return time;
}

void shortest_time(Graph &G, Node* s, std::map<Node*, double> &arrival_time, std::map<Node*, Node*> &parent, std::map<std::string, bool> &forbidden_roads) {
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp_d> unknown;

    if (s->isRestricted()){
        return ;
    }

    // Initialization...
    int n = G.V;

    // sp.assign(n, INF);
    // parentassign(n, nullptr);
    // Can be needed if sp and parent are defined as vectors...

    arrival_time[s] = 0.0;
    unknown.push({s, 0.0});

    while (!unknown.empty()){
        std::pair<Node*, double> v = unknown.top();
        unknown.pop();

        if (v.second > arrival_time[v.first]){
            continue;
        }

        for (Edge* e : G.adj[v.first->getid()]){
            if (e->get_dest()->isRestricted() || e->isRestricted() || forbidden_roads[e->getType()]){
                continue; // Skip if either the Edge or the destination Node is restricted...
            }

            double travel_time = get_travel_time(e, v.second);
            double reach_time = v.second + travel_time;
            
            if (arrival_time.find(e->get_dest()) == arrival_time.end() || reach_time < arrival_time[e->get_dest()]){
                // Updating shortest path
                arrival_time[e->get_dest()] = reach_time;

                // Updating parent
                parent[e->get_dest()] = v.first;

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({e->get_dest(), reach_time});
            }
        }
    }
}