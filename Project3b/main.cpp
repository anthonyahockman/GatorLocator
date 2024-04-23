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
        double lon = stod(components[1]);
        double lat = stod(components[2]);
//        if (!graph.isNode(id)) {
//            continue;
//        }
        graph.insertNode(id, lon, lat);
    }

    file.close();
    cout << "success1" << endl;

     //TODO: Use this to create a .csv file of nodes
//    ofstream file3("test.csv");
//    if (!file3.is_open()) {
//        std::cerr << "Error opening file" << std::endl;
//        return 1;
//    }
//    file3 << "Node id, latitude, longitude, adjacent ids\n";
//    for (auto& n : graph.nodes) {
//        file3 << n.first << "," << n.second->latitude << "," << n.second->longitude << ",";
//        for (auto& n2 : graph.adjacency[n.first]) {
//            file3 << n2.first << " ";
//        }
//        file3 << "\n";
//    }
//    file3.close();

//    float num1 = 30.477;
//    float num2 = -81.5508;
//    float num3 = 29.6561;
//    float num4 = -82.3242;
//    pair<float, float> p1 = make_pair(num1, num2);
//    pair<float, float> p2 = make_pair(num3, num4);
    //TODO: this is the test code for dijkstra
//    long long num1 = 5307728890;
//    long long num2 = 27049669;
//    double num1 = 29.652197899999997;
//    double num2 = -82.349704;
//    double num3 = 29.6531907;
//    double num4 = -82.3462934;
//    pair<double, double> p1 = make_pair(num1, num2);
//    pair<double, double> p2 = make_pair(num3, num4);
//    vector<pair<float, float>> testy = graph.dijkstraSearch(num1, num2);
//    for (auto& v : testy) {
//        cout << v.first << " " << v.second << ",";
//    }

    return 0;
}
