#include <iostream>
#include <vector>
#include "hospital.hpp"
#include "navigation.hpp"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define BLUE    "\033[1;34m"
#define YELLOW  "\033[1;33m"

int main() {
    Hospital hospital;

    int roomCount = hospital.getRoomCount();
    Navigation nav(roomCount);

    // Define room connections (example)
    nav.addConnection(1, 2); // Emergency <-> Radiology
    nav.addConnection(1, 3); // Emergency <-> Pediatrics
    nav.addConnection(2, 4); // Radiology <-> ICU
    nav.addConnection(3, 4); // Pediatrics <-> ICU

    while (true) {
        std::cout << BLUE << "\n===== HOSPITAL NAVIGATION SYSTEM =====\n" << RESET;
        hospital.displayRooms();
        hospital.displayMap();

        int start, end;
        std::cout << "\nEnter Start Room ID (0 to exit): ";
        std::cin >> start;
        if (start == 0) break;

        std::cout << "Enter End Room ID: ";
        std::cin >> end;

        // Get path
        std::vector<int> path = nav.findShortestPath(start, end);

        if (path.empty()) {
            std::cout << RED << "No path found between rooms " << start << " and " << end << RESET << "\n";
        } else {
            std::cout << GREEN << "Shortest Path: ";
            for (size_t i = 0; i < path.size(); i++) {
                Room r = hospital.getRoomById(path[i]);
                std::cout << r.name;
                if (i != path.size() - 1) std::cout << " -> ";
            }
            std::cout << RESET << "\n";
        }

        std::cout << "\n-------------------------------------\n";
    }

    std::cout << "\nExiting Hospital Navigation. Stay Safe!\n";
    return 0;
}
