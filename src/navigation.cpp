#include "navigation.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>

Navigation::Navigation(int totalRooms) {
    roomCount = totalRooms;
    adjList.resize(roomCount + 1); // 1-based indexing
}

void Navigation::addConnection(int room1, int room2, double distance) {
    adjList[room1].push_back({room2, distance});
    adjList[room2].push_back({room1, distance}); // bidirectional
}

// Dijkstra shortest path
std::vector<int> Navigation::findShortestPath(int startId, int endId, double &totalDistance) {
    std::vector<double> dist(roomCount + 1, std::numeric_limits<double>::max());
    std::vector<int> parent(roomCount + 1, -1);
    using pii = std::pair<double, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[startId] = 0;
    pq.push({0, startId});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (u == endId) break;

        for (auto &edge : adjList[u]) {
            int v = edge.to;
            double w = edge.distance;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    totalDistance = dist[endId];
    std::vector<int> path;
    if (dist[endId] == std::numeric_limits<double>::max()) return path; // no path

    for (int v = endId; v != -1; v = parent[v])
        path.push_back(v);

    std::reverse(path.begin(), path.end());
    return path;
}
