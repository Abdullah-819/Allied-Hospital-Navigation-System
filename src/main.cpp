#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"

struct Doctor {
    std::string name, degree, university, age, specialization;
    std::string department, position, experience, additionalInfo;
};

// Load doctors from file
std::vector<Doctor> loadDoctorsFromFile(const std::string& filename) {
    std::vector<Doctor> doctors;
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << RED << "Error: Could not open doctor file!" << RESET << "\n";
        return doctors;
    }
    std::string line;
    while(std::getline(file,line)) {
        std::stringstream ss(line);
        Doctor d;
        std::getline(ss,d.name,'|');
        std::getline(ss,d.degree,'|');
        std::getline(ss,d.university,'|');
        std::getline(ss,d.age,'|');
        std::getline(ss,d.specialization,'|');
        std::getline(ss,d.department,'|');
        std::getline(ss,d.position,'|');
        std::getline(ss,d.experience,'|');
        std::getline(ss,d.additionalInfo,'|');
        doctors.push_back(d);
    }
    return doctors;
}

// Display doctor list
void displayDoctorList(const std::vector<Doctor>& doctors) {
    std::cout << "\n=== Allied Hospital Doctors ===\n";
    for(size_t i=0;i<doctors.size();i++)
        std::cout << i+1 << ". " << doctors[i].name << " (" << doctors[i].department << ")\n";
    std::cout << "0. Back to Main Menu\n";
}

// Display doctor details
void displayDoctorDetails(const Doctor& d) {
    std::cout << GREEN << "\n--- Doctor Details ---\n" << RESET;
    std::cout << "Name: " << d.name << "\n";
    std::cout << "Degree: " << d.degree << "\n";
    std::cout << "University: " << d.university << "\n";
    std::cout << "Age: " << d.age << "\n";
    std::cout << "Specialization: " << d.specialization << "\n";
    std::cout << "Department: " << d.department << "\n";
    std::cout << "Position: " << d.position << "\n";
    std::cout << "Experience: " << d.experience << "\n";
    std::cout << "Additional Info: " << d.additionalInfo << "\n";
}

// View doctors menu
void viewDoctorsMenu(const std::vector<Doctor>& doctors) {
    int choice;
    while(true) {
        displayDoctorList(doctors);
        std::cout << "\nSelect a doctor to view details (0 to go back): ";
        std::cin >> choice;
        if(choice==0) break;
        if(choice>0 && choice<=static_cast<int>(doctors.size()))
            displayDoctorDetails(doctors[choice-1]);
        else
            std::cout << RED << "Invalid choice! Try again.\n" << RESET;
    }
}

// Departments
struct Department { int id; std::string name; };
std::vector<Department> departments = {
    {1,"Emergency"},{2,"Radiology"},{3,"Pediatrics"},{4,"ICU"},
    {5,"Orthopaedics"},{6,"Neurology"},{7,"Plastic Surgery"},{8,"Anaesthesia"}
};

// Adjacency matrix (global for pathfinding)
double dist[9][9] = {0}; // 1-based index

void initializeDistances() {
    dist[1][2]=50; dist[2][1]=50;
    dist[1][3]=60; dist[3][1]=60;
    dist[2][4]=70; dist[4][2]=70;
    dist[3][4]=80; dist[4][3]=80;
    dist[4][5]=90; dist[5][4]=90;
    dist[5][6]=60; dist[6][5]=60;
    dist[6][7]=70; dist[7][6]=70;
    dist[7][8]=50; dist[8][7]=50;
}

// Dijkstra shortest path
std::vector<int> findShortestPath(int start,int end,double &totalDistance){
    int n = 8;
    std::vector<double> distance(n+1,std::numeric_limits<double>::max());
    std::vector<int> prev(n+1,-1);
    distance[start] = 0;
    using PDI = std::pair<double,int>;
    std::priority_queue<PDI,std::vector<PDI>,std::greater<PDI>> pq;
    pq.push({0,start});
    while(!pq.empty()){
        auto [dist_u,u] = pq.top(); pq.pop();
        if(dist_u>distance[u]) continue;
        for(int v=1;v<=n;v++){
            if(dist[u][v]>0){
                double alt = distance[u]+dist[u][v];
                if(alt<distance[v]){
                    distance[v]=alt;
                    prev[v]=u;
                    pq.push({alt,v});
                }
            }
        }
    }
    totalDistance = distance[end];
    std::vector<int> path;
    if(totalDistance==std::numeric_limits<double>::max()) return path;
    for(int at=end;at!=-1;at=prev[at]) path.push_back(at);
    std::reverse(path.begin(),path.end());
    return path;
}

// Print path with colored arrows
void printPathWithArrows(const std::vector<int>& path){
    for(size_t i=0;i<path.size();i++){
        std::cout << GREEN << departments[path[i]-1].name << RESET;
        if(i!=path.size()-1) std::cout << CYAN << " ---> " << RESET;
    }
    std::cout << "\n";
}

// View Departments menu
void viewDepartmentsMenu(){
    while(true){
        std::cout << "\n=== Hospital Departments ===\n";
        for(auto &d : departments) std::cout << d.id << ". " << d.name << "\n";
        std::cout << "0. Back to Main Menu\n";

        int start,end;
        std::cout << "\nEnter Start Department ID: ";
        std::cin >> start;
        if(start==0) break;
        std::cout << "Enter End Department ID: ";
        std::cin >> end;

        double totalDistance=0;
        std::vector<int> path=findShortestPath(start,end,totalDistance);

        if(path.empty()){
            std::cout << RED << "No path exists!\n" << RESET;
            continue;
        }

        std::cout << "\nShortest Path:\n";
        printPathWithArrows(path);
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Total Distance: " << totalDistance << " meters (" << totalDistance/1000 << " km)\n";
        double normalTime=totalDistance/1.4; // seconds walking normally
        double fastTime=totalDistance/2.0;
        std::cout << "Estimated Time: Normal walk: " << normalTime/60 << " min, Fast walk: " << fastTime/60 << " min\n";
    }
}

int main(){
    initializeDistances(); // Important! set the adjacency matrix
    std::vector<Doctor> doctors=loadDoctorsFromFile("private/doctors.txt");
    int mainChoice;
    while(true){
        std::cout << BLUE << "\n===== WELCOME TO ALLIED HOSPITAL =====\n" << RESET;
        std::cout << "1. View Departments\n";
        std::cout << "2. View Doctors\n";
        std::cout << "3. Register / Complain\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> mainChoice;
        switch(mainChoice){
            case 1: viewDepartmentsMenu(); break;
            case 2: viewDoctorsMenu(doctors); break;
            case 3: std::cout << YELLOW << "Register/Complain is under development.\n" << RESET; break;
            case 4: std::cout << GREEN << "Exiting. Stay Safe!\n" << RESET; return 0;
            default: std::cout << RED << "Invalid choice! Try again.\n" << RESET;
        }
    }
}
