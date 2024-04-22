#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "AdjacencyList.cpp"

using namespace std;

//TODO: issues below
//Directions vector for each way seem to only have one set of lat long
//Turns out this probably isn't an issue -- OSM treats POIs as ways to keep things consistent

int main() {

    AdjacencyList graph;

    ifstream file2("cmake-build-debug/edges.csv");
    string line2;
    // deal with column headers
    getline(file2, line2);

    while (getline(file2, line2)) {
        vector<string> components;
        stringstream ss2(line2);
        string component;

        // only up to linestring data
        for (int i = 0; i < 8; i++) {
            getline(ss2, component, ',');
            components.push_back(component);
        }
        for (int j = 0; j < 5; j++) {
            getline(ss2, component, ',');
        }
        getline(ss2, component, ')');
        // if the way entry is a copy of another one, skip it
        if (components[0].find("-0")==string::npos) {
            continue;
        }
        // if the "way" is actually just a POI, skip it
        if (component.find(',')==string::npos) {
            continue;
        }
        // if the way is not driveable, skip it
        if (components[6] == "Forbidden") {
            continue;
        }

        long long from = stoll(components[2]);
        long long to = stoll(components[3]);
        long long osm_id = stoll(components[1]);
        double length = stod(components[4]);
        string type = components[6];

        graph.insertEdge(from, to, osm_id, length, type);

    }
    file2.close();
    cout << "success!" << endl;

    ifstream file("cmake-build-debug/nodes.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file\n";
        return 1;
    }
    string line;
    // deal with column headers
    getline(file, line);
    while (getline(file, line)) {
        vector<string> components;
        stringstream ss(line);
        string component;

        while (getline(ss, component, ',')) {
            components.push_back(component);
        }
        long long id = stoll(components[0]);
        float lon = stof(components[1]);
        float lat = stof(components[2]);
//        if (!graph.isNode(id)) {
//            continue;
//        }
        graph.insertNode(id, lon, lat);
    }

    file.close();
    cout << "success1" << endl;

    return 0;
}
