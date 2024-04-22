//
// Created by antho on 4/20/2024.
//
#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    // keep in case we switch to a vector data structure to initialize it
}

bool AdjacencyList::insertNode(long long &id, float &longitude, float &latitude) {
    if (isNode(id)) {
        Node* Node1 = new Node;
        Node1->id = id;
        Node1->longitude = longitude;
        Node1->latitude = latitude;
        unordered_map<long long, long long> map1;
        adjacency.emplace(id, map1);
        nodes.emplace(id, Node1);
        return true;
    }
    return false;
}

bool AdjacencyList::insertEdge(long long &from, long long &to, long long &osm_id, double &length, string &type) {
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

bool AdjacencyList::isEdge(long long &osm_id) {
    if (edges.find(osm_id) != edges.end()) {
        return true;
    }
    return false;
}

// TODO: this needs to be tested
vector<pair<float, float>> AdjacencyList::dijkstraSearch(pair<float, float> start, pair<float, float> end) {
    long long startID;
    long long endID;
    for (const auto& pair: nodes) {
        if (pair.second->latitude == start.first && pair.second->longitude == start.second) {
            startID = pair.second->id;
        }
        if (pair.second->latitude == end.first && pair.second->longitude == end.second) {
            endID = pair.second->id;
        }
    }
    typedef pair<double, long long> w;
    priority_queue<w, vector<w>, greater<>> pq;
    unordered_map<long long, double> distance;
    distance.emplace(startID, 0);

    pq.emplace(0, startID);
    unordered_map<long long, long long> parent;

    while (!pq.empty()) {

        long long id = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        if (d > distance[id])
            continue;

        for (auto& neighbor : adjacency[id]) {
            long long v = neighbor.first;
            long long osm = neighbor.second;
            double w = edges[osm]->length;
            if (distance[id] + w < distance[v]) {
                distance[v] = distance[id] + w;
                parent[v] = id;
                pq.emplace(distance[v], v);
            }
        }
    }
    vector<long long> path;
    // TODO: insert end point
    for (long long v = endID; v != startID; v = parent[v]) {
        path.push_back(v);
    }
    path.push_back(startID);
    reverse(path.begin(), path.end());

    vector<pair<float, float>> path_ll;

    for (auto& v : path) {
        double lat = nodes[v]->latitude;
        double lon = nodes[v]->longitude;
        path_ll.emplace_back(lat, lon);
    }

    return path_ll;
}

