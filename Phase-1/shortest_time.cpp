#include "graph.hpp"

#ifndef INF
#define INF 1000000000
#endif

#define TOT_SECONDS 86400
#define INTERVAL_TIME 900

struct cmp{
    bool operator()(std::pair<Node*, double>& a, std::pair<Node*, double>& b){
        return a.second > b.second;
    }
};

double get_travel_time(Graph &G, Node* start, Node* end, double arrival_time, const std::vector<double>& profile) {
    double t = arrival_time - ((int)arrival_time) / TOT_SECONDS;
    double distance = G.distance(start, end);
    double time = 0;

    int interval = ((int)t) / INTERVAL_TIME;
    double dist_left = distance;
    double speed = 0.0;
    double time_left_in_interval = INTERVAL_TIME - (t - interval*INTERVAL_TIME);
    int i=interval;
    while (dist_left>0) {
        if (i!=interval) {time_left_in_interval = INTERVAL_TIME;}
        speed = profile[i];
        if (speed*time_left_in_interval >= dist_left) {
            time += dist_left/speed;
            dist_left = 0;
        }
        else {
            time += time_left_in_interval;
            dist_left -= speed*time_left_in_interval;
            i++;  
        }
    }
    return time;
}

void shortest_time(Graph &G, Node* s, std::map<Node*, double> &arrival_time, std::map<Node*, Node*> &parent, const std::vector<double>& profile) {
    std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, cmp> unknown;

    // Initialization...
        int n = G.V;

        // sp.assign(n, INF);
        // parentassign(n, nullptr);
        // Can be needed if sp and parent are defined as vectors...

        arrival_time[s] = 0;
        unknown.push({s, 0});

        while (!unknown.empty()){
            std::pair<Node*, double> v = unknown.top();
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
