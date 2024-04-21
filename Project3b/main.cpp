#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "AdjacencyList.h"

using namespace std;

//TODO: issues below
//Directions vector for each way seem to only have one set of lat long
//Turns out this probably isn't an issue -- OSM treats POIs as ways to keep things consistent

int main() {

    AdjacencyList graph;

    ifstream file("nodes.csv");
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
        graph.insertNode(id, lon, lat);
    }

    file.close();
    cout << "success1" << endl;

    ifstream file2("edges.csv");
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
        stringstream ss3(component);
        string nums;
        list<pair<float, float>> dirs;
        while (getline(ss3, nums, ',')) {
            string num1;
            string num2;
            stringstream ss4(nums);
            getline(ss4, num1, ' ');
            getline(ss4, num1, ' ');
            getline(ss4, num2, ' ');
            float lon = stof(num1);
            float lat = stof(num2);
            dirs.emplace_back(lon, lat);
        }

        long long from = stoll(components[2]);
        long long to = stoll(components[3]);
        long long osm_id = stoll(components[1]);
        double length = stod(components[4]);
        string type = components[6];

        graph.insertEdge(from, to, osm_id, length, type, dirs);

    }
    cout << "success!" << endl;
    return 0;
}
