//
// Created by antho on 4/20/2024.
//
#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    // keep in case we switch to a vector data structure to initialize it
}

bool AdjacencyList::insertNode(long long &id, double &longitude, double &latitude) {
    if (isNode(id)) {
        Node* Node1 = new Node;
        Node1->id = id;
        Node1->longitude = longitude;
        Node1->latitude = latitude;
        unordered_map<long long, string> map1;
        adjacency.emplace(id, map1);
        nodes.emplace(id, Node1);
        return true;
    }
    return false;
}

bool AdjacencyList::insertEdge(long long &from, long long &to, string &osm_id, double &length, string &type) {
    if (!isEdge(osm_id)) {
        Way* Way1 = new Way;
        Way1->osm_id = osm_id;
        Way1->length = length;
        Way1->type = type;
        //Way1->directions = directions;
        edges.emplace(osm_id, Way1);
        adjacency[from].emplace(to, osm_id);
        adjacency[to].emplace(from, osm_id);
    }
    return false;
}

bool AdjacencyList::isNode(long long &id) {
    if (adjacency.find(id) != adjacency.end()) {
        return true;
    }
    return false;
}

bool AdjacencyList::isEdge(string &osm_id) {
    if (edges.find(osm_id) != edges.end()) {
        return true;
    }
    return false;
}

// TODO: this needs to be tested
vector<pair<float, float>> AdjacencyList::dijkstraSearch(long long startID, long long endID) {
//    long long startID;
//    long long endID;
//    // determine the actual id of each point based on latitude and longitude inputs
//    for (const auto& pair: nodes) {
//        if (pair.second->latitude == start.first && pair.second->longitude == start.second) {
//            startID = pair.second->id;
//        }
//        if (pair.second->latitude == end.first && pair.second->longitude == end.second) {
//            endID = pair.second->id;
//        }
//    }
    // define a type of data for the priority queue
    typedef pair<double, long long> w;
    // start the priority queue
    priority_queue<w, vector<w>, greater<>> pq;
    // keep track of all the distances to each node
    unordered_map<long long, double> distance;

    distance.emplace(startID, 0);

    // initialize the distance map values to large quantities
    for (auto& v : nodes) {
        distance.emplace(v.first, numeric_limits<long long>::max());
    }

    pq.emplace(0, startID);
    // keep track of the parents of each node
    unordered_map<long long, long long> parent;

    // Actual Dijkstra Loop
    while (!pq.empty()) {
        long long id = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        // If the distance is not shorter than the current stored distance, skip
        if (d > distance[id])
            continue;

        for (auto& neighbor : adjacency[id]) {
            long long v = neighbor.first;
            string osm = neighbor.second;
            double w = edges[osm]->length * getSpeed(osm);
            // Only replace if the id distance + weight is less than the current distance to a node/vertex
            if (distance[id] + w < distance[v]) {
                distance[v] = distance[id] + w;
                parent[v] = id;
                pq.emplace(distance[v], v);
            }
        }
    }

    vector<long long> path;
    // add all of the nodes along the path starting from the end
    for (long long v = endID; v != startID; v = parent[v]) {
        path.push_back(v);
    }
    path.push_back(startID);
    // reverse the vector
    reverse(path.begin(), path.end());

    // create a new vector for the longitude, latitude points
    vector<pair<float, float>> path_ll;
    // fill the path_ll vector
    for (auto& v : path) {
        double lat = nodes[v]->latitude;
        double lon = nodes[v]->longitude;
        path_ll.emplace_back(lat, lon);
    }

    return path_ll;
}

int AdjacencyList::getSpeed(string &id) {
    string roadType = edges[id]->type;
    int speed;
    if (roadType == "Motorway") {
        speed = 70;
    }
    else if (roadType == "Primary") {
        speed = 55;
    }
    else if (roadType == "Secondary") {
        speed = 45;
    }
    else if (roadType == "Tertiary") {
        speed = 35;
    }
    else if (roadType == "Residential") {
        speed = 25;
    }
    else {
        speed = 40;
    }
    return speed;
}

