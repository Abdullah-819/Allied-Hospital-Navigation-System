#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <vector>
#include <queue>
#include <unordered_map>
#include "hospital.hpp"

struct Edge {
    int to;
    double distance; // in meters
};

class Navigation {
private:
    int roomCount;
    std::vector<std::vector<Edge>> adjList; // weighted adjacency list

public:
    Navigation(int totalRooms); // constructor

    void addConnection(int room1, int room2, double distance); // weighted edge
    std::vector<int> findShortestPath(int startId, int endId, double &totalDistance); // Dijkstra
};

#endif
