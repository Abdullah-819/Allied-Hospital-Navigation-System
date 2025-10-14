#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <vector>
#include <queue>
#include <unordered_map>
#include "hospital.hpp"

class Navigation {
private:
    int roomCount;
    std::vector<std::vector<int>> adjList; // adjacency list for rooms

public:
    Navigation(int totalRooms); // constructor

    void addConnection(int room1, int room2); // add edge between rooms
    std::vector<int> findShortestPath(int startId, int endId); // BFS shortest path
};

#endif
