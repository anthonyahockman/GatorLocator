//
// Created by antho on 4/20/2024.
//
#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    // keep in case we switch to a vector data structure to initialize it
}

bool AdjacencyList::insertNode(long long &id, double &longitude, double &latitude) {
    if (isNode(id)) {//check that the node is in the list
        Node* Node1 = new Node;//define a new node
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

//attempts to insert an edge into the adjacency list
bool AdjacencyList::insertEdge(long long &from, long long &to, string &osm_id, double &length, string &type) {
    if (!isEdge(osm_id)) {//check that it is not an edge first
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

//determines if the given osm id is a node in the adjacency list
bool AdjacencyList::isNode(long long &id) {
    if (adjacency.find(id) != adjacency.end()) {
        return true;
    }
    return false;
}

//determines if the given osmID is an edge in the edge list
bool AdjacencyList::isEdge(string &osm_id) {
    if (edges.find(osm_id) != edges.end()) {
        return true;
    }
    return false;
}

vector<pair<float, float>> AdjacencyList::dijkstraSearch(long long startID, long long endID) {

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

vector<pair<float, float>> AdjacencyList::AStar(long long startID, long long endID) {

    //Maps have keys that are ID's and values that are the heurisitc function value, f(x)
    unordered_map<long long, float> open;//calculated
    unordered_map<long long, float> closed;//to be calculated
    unordered_map<long long, float> gValues;
    unordered_map<long long, long long> parent;


    //Assign the max float value as inf for each g(x) value
    for (auto& v : nodes) {
        gValues.emplace(v.first, numeric_limits<float>::max());
    }

    //place the starting node in the open list with an f(x) value of 0
    open.emplace(startID,0);//f is 0
    gValues[startID] = 0;

    //While the open list is not empty
    while (!open.empty()) {
        long long currentNode = open.begin()->first;//current node to explore
        float minF = open[currentNode];//f(x) value for the current node

        //determine if any other nodes in open have a smaller f(x) value
        for (auto p : open) {
            if (p.second < minF) {
                currentNode = p.first;
                minF = p.second;
            }
        }

        //remove the current node from open
        open.erase(currentNode);

        //if the current node is the end of the path, we are done
        if (currentNode == endID) {
            break;
        }

        //for each adjacent node connect to the current node by an edge
        for (auto successor : adjacency[currentNode]) {
            //calculate the total cost of travelling to it
            float successorCost = gValues[currentNode] + edges[successor.second]->length * getSpeed(successor.second);

            //Check if the node is in open
            if (open.find(successor.first) != open.end()) {
                if (gValues[successor.first] <= successorCost) {//if the current travel cost is less than the new one
                    continue;//forget it
                }
            } else if (closed.find(successor.first) != closed.end()) {//check that the node is in closed
                if (gValues[successor.first] <= successorCost) {//if the current travel cost is less than the new one
                    continue;//forget it
                }
                //move from closed to open and update f(x) = g(x) + h(x)
                closed.erase(successor.first);
                open.emplace(successor.first,successorCost + abs(nodes[successor.first]->longitude - nodes[endID]->longitude) + abs(nodes[successor.first]->latitude - nodes[endID]->latitude));
            } else {
                //add to open and update f(x) = g(x) + h(x)
                open.emplace(successor.first,successorCost + abs(nodes[successor.first]->longitude - nodes[endID]->longitude) + abs(nodes[successor.first]->latitude - nodes[endID]->latitude));
            }
            //update the g values
            gValues[successor.first] = successorCost;
            //assign the current node as the parent to the adjacent one
            parent[successor.first] = currentNode;
        }
        //add the current node to clsoed with its f(x) value
        closed.emplace(currentNode,minF);
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

//gets the speed liekly speed limit of the edge using the type of edge
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

