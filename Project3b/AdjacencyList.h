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
    // store nodes with list of neighors
    // each pair has the node belonging to the key id and the adjacent ids
    unordered_map<long long, pair<Node*, unordered_map<long long, long long>>> adjacency;
    // store the list of edges with edge_id as the key
    // the list is the latitude, longitude way data from OSM
    unordered_map<long long, Way*> edges;
public:
    AdjacencyList();
    bool insertNode(long long &id, float &longitude, float &latitude);
    bool insertEdge(long long &from, long long &to, long long &osm_id, double &length, string &type);
    bool isNode(long long &id);
    bool isEdge(long long &osm_id);
};


#endif //PROJECT3B_ADJACENCYLIST_H
