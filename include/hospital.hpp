#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <string>
#include <vector>

struct Room {
    int id;
    std::string name;
    int x, y; 
};

class Hospital {
private:
    std::vector<Room> rooms;

public:
    Hospital(); 

    void displayRooms();   
    void displayMap();     
    int getRoomCount();
    Room getRoomById(int id);
};

#endif
