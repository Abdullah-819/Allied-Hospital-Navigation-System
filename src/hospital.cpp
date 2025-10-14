#include "hospital.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"

Hospital::Hospital() {
    rooms.push_back({1, "Emergency", 0, 0});
    rooms.push_back({2, "Radiology", 0, 1});
    rooms.push_back({3, "Pediatrics", 1, 0});
    rooms.push_back({4, "ICU", 1, 1});
}

void Hospital::displayRooms() {
    std::cout << BLUE << "Rooms:\n" << RESET;
    for (const auto& room : rooms) {
        std::cout << room.id << ". " << room.name << "\n";
    }
}

void Hospital::displayMap() {
    std::cout << GREEN << "\nASCII Hospital Map:\n" << RESET;
    std::cout << "[" << RED << "1" << RESET << "]---[" << BLUE << "2" << RESET << "]\n";
    std::cout << " |       |\n";
    std::cout << "[" << YELLOW << "3" << RESET << "]---[" << GREEN << "4" << RESET << "]\n";
}

int Hospital::getRoomCount() {
    return rooms.size();
}

Room Hospital::getRoomById(int id) {
    for (auto& room : rooms) {
        if (room.id == id) return room;
    }
    return {-1, "Unknown", -1, -1};
}
