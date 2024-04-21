//
// Created by antho on 4/20/2024.
//
#pragma once
#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    // keep in case we switch to a vector data structure to initialize it
}

bool AdjacencyList::insertNode(long long &id, float &longitude, float &latitude) {
    if (!isNode(id)) {
        Node* Node1 = new Node;
        Node1->id = id;
        Node1->longitude = longitude;
        Node1->latitude = latitude;
        unordered_map<long long, long long> map1;
        adjacency.emplace(id, make_pair(Node1, map1));
        return true;
    }
    return false;
}

bool AdjacencyList::insertEdge(long long &from, long long &to, long long &osm_id, double &length, string &type, list<pair<float, float>> &directions) {
    if (!isEdge(osm_id)) {
        Way* Way1 = new Way;
        Way1->osm_id = osm_id;
        Way1->length = length;
        Way1->type = type;
        Way1->directions = directions;
        edges.emplace(osm_id, Way1);
        adjacency[from].second.emplace(to, osm_id);
        adjacency[to].second.emplace(from, osm_id);
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

