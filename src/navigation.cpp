#include "navigation.hpp"
#include <iostream>
#include <algorithm>

// Constructor
Navigation::Navigation(int totalRooms) {
    roomCount = totalRooms;
    adjList.resize(roomCount + 1); // 1-based indexing
}

// Add bidirectional connection
void Navigation::addConnection(int room1, int room2) {
    adjList[room1].push_back(room2);
    adjList[room2].push_back(room1);
}

// BFS to find shortest path
std::vector<int> Navigation::findShortestPath(int startId, int endId) {
    std::vector<int> parent(roomCount + 1, -1);
    std::vector<bool> visited(roomCount + 1, false);
    std::queue<int> q;

    visited[startId] = true;
    q.push(startId);

    while (!q.empty()) {
        int curr = q.front(); q.pop();

        if (curr == endId) break;

        for (int neighbor : adjList[curr]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = curr;
                q.push(neighbor);
            }
        }
    }

    // Reconstruct path
    std::vector<int> path;
    if (!visited[endId]) return path; // no path

    for (int v = endId; v != -1; v = parent[v])
        path.push_back(v);

    std::reverse(path.begin(), path.end());
    return path;
}
