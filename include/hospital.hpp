#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <string>
#include <vector>

// Room struct
struct Room {
    int id;
    std::string name;
    int x, y; // optional for map coordinates
};

// Hospital class
class Hospital {
private:
    std::vector<Room> rooms;

public:
    Hospital(); // Constructor to initialize rooms

    void displayRooms();   // Print room menu
    void displayMap();     // Print ASCII hospital map
    int getRoomCount();
    Room getRoomById(int id);
};

#endif
