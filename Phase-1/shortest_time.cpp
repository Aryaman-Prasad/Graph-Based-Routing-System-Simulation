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



double get_travel_time(Graph &G, Node* start, Node* end, double arrival_time, const std::vector<bool>& profile) {
    double dist = G.distance(start, end);
    double t = arrival_time - arrival_time / TOT_SECONDS;
    double distance = G.distance(start, end);
    double time = 0;
    //Assuming that all of a person's travels get over in a day!

    int interval = t / INTERVAL_TIME;
    double dist_left = distance;
    double speed = 0.0;
    double time_left_in_interval = INTERVAL_TIME - (t - interval*INTERVAL_TIME);
    for (int i=interval; i<(TOT_SECONDS/INTERVAL_TIME); i++) {
        if (i!=interval) {time_left_in_interval = INTERVAL_TIME;}
        speed = profile[i];
        if (speed*time_left_in_interval >= dist_left) {
            time += dist_left/speed;
            return time;
        }
        time += time_left_in_interval;
        dist_left -= speed*time_left_in_interval;
    }
}

void shortest_time(Graph &G, Node* s, std::map<Node*, int> &arrival_time, std::map<Node*, Node*> &parent, const std::vector<bool>& profile) {
    std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, cmp> unknown;

    // Initialization...
        int n = G.V;

        // sp.assign(n, INF);
        // parentassign(n, nullptr);
        // Can be needed if sp and parent are defined as vectors...

        arrival_time[s] = 0;
        unknown.push({s, 0});

        while (!unknown.empty()){
            std::pair<Node*, int> v = unknown.top();
            unknown.pop();

            if (v.second > arrival_time[v.first]){
                continue;
            }

            for (Edge* e : G.adj[v.first->getid()]){
                if (e->get_dest()->isRestricted() || e->isRestricted()){
                    continue; // Skip if either the Edge or the destination Node is restricted...
                }
                double travel_time = get_travel_time(G,v.first,e->get_dest(),v.second,profile);
                double reach_time = v.second + travel_time;
                
                if (reach_time < arrival_time[e->get_dest()]){
                    // Updating shortest path
                    arrival_time[e->get_dest()] = reach_time;

                    // Updating parent
                    parent[e->get_dest()] = v.first;

                    // Adding updated element into heap, deleting previous version of it is not necessary (or is it??)
                    unknown.push({e->get_dest(), arrival_time[e->get_dest()]});
                }
            }
        }
}