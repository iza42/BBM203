#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace  std;

Map::Map() {
    // Initializes all distances to a value representing no direct connections
    for (auto & i : distanceMatrix) {
        for (int & j : i) {
            j = -2; // -2 means there is no direct connections before
        }
    }
    // Initializes all provinces as unvisited
    for (bool & b : visited) {
        b=false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    std::ifstream file(filename); // opening  the file
    if (!file.is_open()) {
        std::cerr << "Error: File cannot be opened!" << std::endl;
        return ;
    }
    string line;
    int row=0;
    while (getline(file, line)) {
        if (!line.empty()) {
            if (line.back() == '\r') {
                line.pop_back();
            }
            stringstream ss(line);
            string distance_str;
            int col=0;
            while (std::getline(ss, distance_str, ',')) { // separate the line
                int distance = std::stoi(distance_str);
                distanceMatrix[col++][row]=distance;

            }
            row++;
        }
    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    return (distanceMatrix[provinceA][provinceB] > 0 &&
            distanceMatrix[provinceA][provinceB] <= maxDistance);
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province]=true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (bool &b : visited) {
        b=false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count =0;
    for (bool b : visited) {
       if(b==true) {
           count++;
       }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}