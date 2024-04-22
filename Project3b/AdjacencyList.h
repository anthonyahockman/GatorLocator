//
// Created by antho on 4/20/2024.
//

#ifndef PROJECT3B_ADJACENCYLIST_H
#define PROJECT3B_ADJACENCYLIST_H
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

struct Node {
    long long id;
    float longitude;
    float latitude;
};

struct Way {
    long long osm_id;
    double length;
    string type;
    //list<pair<float, float>> directions;
};

class AdjacencyList {
public:
    // store nodes with list of neighbors
    // the inner map has key of an adjacent node and the oms_id for the edge
    unordered_map<long long, unordered_map<long long, long long>> adjacency;
    // keep track of nodes and corresponding ids
    unordered_map<long long, Node*> nodes;
    // store the list of edges with edge_id as the key
    // the list is the latitude, longitude way data from OSM
    unordered_map<long long, Way*> edges;
public:
    AdjacencyList();
    bool insertNode(long long &id, float &longitude, float &latitude);
    bool insertEdge(long long &from, long long &to, long long &osm_id, double &length, string &type);
    bool isNode(long long &id);
    bool isEdge(long long &osm_id);
    vector<pair<float, float>> dijkstraSearch(long long &startID, long long &endID);
    int getSpeed(long long &id);
};


#endif //PROJECT3B_ADJACENCYLIST_H
