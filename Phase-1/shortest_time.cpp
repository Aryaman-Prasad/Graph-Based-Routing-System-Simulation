#include "common.hpp"

#ifndef INF
#define INF std::numeric_limits<double>::max()
#endif

// Redundant...
// #ifndef TOT_SECONDS
// #define TOT_SECONDS 86400
// #endif

#ifndef INTERVAL_TIME
#define INTERVAL_TIME 900
#endif

#ifndef PROFILE_LEN
#define PROFILE_LEN 96
#endif

// TO get travel time from node to dest along the edge when it arrives at arrival time
double get_travel_time(Edge* e, double arrival_time){
    if (e->getProfile().size() == 0){
        return e->getTime();// For edge with no speed profile
    }

    double dist_left = e->get_length();
    double time = 0;
    int t = 0;

    // calculating time based on current time , speed and distance
    while (dist_left > 0){
        double speed = (e->getProfile())[(t + (int) arrival_time / INTERVAL_TIME) % PROFILE_LEN];
        double interval_time = ((double) INTERVAL_TIME - arrival_time);
        double dist_coverable = speed * interval_time;

        if (dist_coverable >= dist_left){
            time += (dist_left) / speed;
            dist_left = 0;
            continue;
        }

        if (dist_coverable == 0){
            return INF;
        }

        time += (dist_coverable) / speed;
        dist_left -= dist_coverable;
        arrival_time = 0;
        t++;
    }

    return time;
}

void shortest_time(Graph &G, Node* s, int &target, std::vector<double> &arrival_time, std::vector<int> &parent, std::map<std::string, bool> &forbidden_roads) {
    std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<>> unknown;

    if (s->isRestricted()){
        return ;
    }

    arrival_time[s->getid()] = 0.0;
    unknown.push({0.0, s});

    while (!unknown.empty()){
        std::pair<double, Node*> v = unknown.top();// always picking shortest time
        unknown.pop();

        if (v.second->getid() == target){
            return ;
        }

        if (v.first == INF){
            return ;
        }

        if (v.first != arrival_time[v.second->getid()]){
            continue;
        }

        for (Edge* e : G.adj[v.second->getid()]){
            if (e->get_dest()->isRestricted() || e->isRestricted() || forbidden_roads[e->getType()]){
                continue; // Skip if either the Edge or the destination Node is restricted...
            }

            double travel_time = get_travel_time(e, v.first);
            double reach_time = v.first + travel_time;
            
            if (reach_time < arrival_time[e->get_dest()->getid()]){
                // Updating shortest path
                arrival_time[e->get_dest()->getid()] = reach_time;

                // Updating parent
                parent[e->get_dest()->getid()] = v.second->getid();

                // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                unknown.push({reach_time, e->get_dest()});
            }
        }
    }
}