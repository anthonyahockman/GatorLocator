#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "AdjacencyList.cpp"

using namespace std;

int main() {

    AdjacencyList graph;

    cout << "Loading OSM Edge Data... " << endl;

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
        string osm_id = components[0];
        double length = stod(components[4]);
        string type = components[6];

        graph.insertEdge(from, to, osm_id, length, type);

    }
    file2.close();
    cout << "Edge Data Successfully Loaded!\n" << endl;
    cout << "Loading OSM Node Data..." << endl;

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
        double lon = stod(components[1]);
        double lat = stod(components[2]);
        graph.insertNode(id, lon, lat);
    }

    file.close();
    cout << "Node Data Successfully Loaded!\n" << endl;


    cout << "Performing Dijkstra's Search Algorithm..." << endl;
    long long n1 = 1326389497;
    long long n2 = 1326389461;


    vector<pair<float,float>> v = graph.dijkstraSearch(n1,n2);
    cout << "Dijkstra's Search Finished:" << endl;
    for (auto p : v) {
        cout << setprecision(9) << p.first << ", " << p.second << endl;
    }

    cout << "\nPerforming A* Search Algorithm..." << endl;
    v = graph.AStar(n1,n2);
    cout << "A* Search Finished:" << endl;
    for (auto p : v) {
        cout << setprecision(9) << p.first << ", " << p.second << endl;
    }

    return 0;
}
