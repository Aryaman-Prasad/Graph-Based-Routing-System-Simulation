#include "graph.hpp"

using json = nlohmann::json;

// NODE Class Functions //
int Node::getid(){
    return id;
}

double Node::get_lat(){
    return lat;
}

double Node::get_lon(){
    return lon;
}

bool Node::isRestricted(){
    return restricted;
}

void Node::updateRestriction(bool d){
    restricted = d;
}

bool Node::check_poi(std::string poi){
    for (auto s : pois){
        if (s == poi){
            return true;
        }
    }

    return false;
}

// EDGE Class Functions //
int Edge::getId(){
    return id;
}

Node* Edge::get_dest(){
    return dest;
}

double Edge::get_length(){
    return length;
}

std::string Edge::getType(){
    return road_type;
}

double Edge::getTime(){
    return avg_time;
}

std::vector<double> Edge::getProfile(){
    return speed_profile;
}

void Edge::update_length(double len){
    length = len;
}

void Edge::update_avg_time(double time){
    avg_time = time;
}

bool Edge::isRestricted(){
    return restricted;
}

bool Edge::isOneway(){
    return oneway;
}

void Edge::updateRestriction(bool d){
    restricted = d;
}

void Edge::updateProfile(std::vector<double> newProfile){
    speed_profile = newProfile;
}

void Edge::updateType(std::string newType){
    road_type = newType;
}

// GRAPH Class functions //
Node* Graph::getNode(int id){
    return vertices[id];
}

double Graph::distance(std::pair<double, double> v1, Node* v2){
    double dx = v1.first - v2->get_lat();
    double dy = v1.second - v2->get_lon();
    return sqrt(dx*dx + dy*dy);
}

void Graph::addNode(Node* v){
    vertices[v->getid()] = v;
}

void Graph::removeNode(Node* v){
    // Marking vector to be unused since node can be restored in later queries
    vertices[v->getid()]->updateRestriction(false);
}

void Graph::addEdge(Node* v, Edge* e, bool d){
    adj[v->getid()].push_back(e);
    if (d) {
        edges[e->getId()] = {v->getid(), e->get_dest()->getid()}; // In case of undirected edges, addEdge() would be called twice but we want addition into map only once, hence the boolean variable
    }
    E++;
}

bool Graph::removeEdge(int edge_id){
    std::pair<int, int> vers = edges[edge_id]; // Find the pair of vertices whose edge needs to be marked
    bool m = false; // Marker to check if the edge actually got removed or not

    // Iterating over edges w.r.t first node
    for (Edge* e : adj[vers.first]){
        if (e->getId() == edge_id){
            e->updateRestriction(true); // Mark edge
            m = true;
            break;
        }
    }

    // Iterating over edges w.r.t second node, if edge is oneway then no match might be found but that is fine
    for (Edge* e : adj[vers.second]){
        if (e->getId() == edge_id){
            e->updateRestriction(true); // Mark edge
            m = true;
            break;
        }
    }

    return m;
}

bool Graph::modifyEdge(int edge_id, json &patch){
    if (edges.find(edge_id) == edges.end()){
        return false;
    }
    std::pair<int, int> vers = edges[edge_id];
    bool m = false;

    // Iterating over edges w.r.t first node
    for (Edge* e : adj[vers.first]){
        if (e->getId() == edge_id){
            e->updateRestriction(false);
            m = true;
            if (patch.find("length") != patch.end()){
                e->update_length(patch["length"]);
            }
            if (patch.find("average_time") != patch.end()){
                e->update_avg_time(patch["average_time"]);
            }
            if (patch.find("speed_profile") != patch.end()){
                e->updateProfile(patch["speed_profile"]);
            }
            if (patch.find("road_type") != patch.end()){
                e->updateType(patch["road_type"]);
            }
            break;
        }
    }

    // Iterating over edges w.r.t second node
    for (Edge* e : adj[vers.second]){
        if (e->getId() == edge_id){
            e->updateRestriction(false);
            m = true;
            if (patch.find("length") != patch.end()){
                e->update_length(patch["length"]);
            }
            if (patch.find("average_time") != patch.end()){
                e->update_avg_time(patch["average_time"]);
            }
            if (patch.find("speed_profile") != patch.end()){
                e->updateProfile(patch["speed_profile"]);
            }
            if (patch.find("road_type") != patch.end()){
                e->updateType(patch["road_type"]);
            }
            break;
        }
    }

    return m;
}
