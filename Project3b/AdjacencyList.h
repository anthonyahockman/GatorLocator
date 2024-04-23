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
    double longitude;
    double latitude;
};

struct Way {
    string osm_id;
    double length;
    string type;
    //list<pair<float, float>> directions;
};

class AdjacencyList {
public:
    // store nodes with list of neighbors
    // the inner map has key of an adjacent node and the oms_id for the edge
    unordered_map<long long, unordered_map<long long, string>> adjacency;
    // keep track of nodes and corresponding ids
    unordered_map<long long, Node*> nodes;
    // store the list of edges with edge_id as the key
    // the list is the latitude, longitude way data from OSM
    unordered_map<string, Way*> edges;
public:
    AdjacencyList();
    bool insertNode(long long &id, double &longitude, double &latitude);
    bool insertEdge(long long &from, long long &to, string &osm_id, double &length, string &type);
    bool isNode(long long &id);
    bool isEdge(string &osm_id);
    vector<pair<float, float>> dijkstraSearch(long long startID, long long endID);
    vector<pair<float, float>> AStar(long long startID, long long endID);
    int getSpeed(string &id);
};


#endif //PROJECT3B_ADJACENCYLIST_H
