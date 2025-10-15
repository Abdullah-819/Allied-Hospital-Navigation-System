#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"

using namespace std;

// ========================== STRUCTURES ==========================
struct Doctor {
    string name, degree, university, age, specialization;
    string department, position, experience, additionalInfo;
};

struct Department { int id; string name; };

// ========================== GLOBAL VARIABLES ==========================
vector<Department> departments = {
    {1,"Emergency"},{2,"Radiology"},{3,"Pediatrics"},{4,"ICU"},
    {5,"Orthopaedics"},{6,"Neurology"},{7,"Plastic Surgery"},{8,"Anaesthesia"}
};

double dist[9][9] = {0}; // adjacency matrix (1-based)

// LOGIN SYSTEM (default credentials)
map<string,string> adminAccounts = { {"admin","admin123"} };
map<string,string> userAccounts  = { {"user","user123"} };
bool isAdmin = false;
bool isUser = false;

// ========================== FUNCTIONS ==========================

// Initialize distances between departments (call once at startup)
void initializeDistances() {
    // zeroed by global initializer; set real connections:
    dist[1][2]=50; dist[2][1]=50;
    dist[1][3]=60; dist[3][1]=60;
    dist[2][4]=70; dist[4][2]=70;
    dist[3][4]=80; dist[4][3]=80;
    dist[4][5]=90; dist[5][4]=90;
    dist[5][6]=60; dist[6][5]=60;
    dist[6][7]=70; dist[7][6]=70;
    dist[7][8]=50; dist[8][7]=50;
}

// Load doctors from file (pipe '|' separated)
// file path example: "private/doctors.txt"
vector<Doctor> loadDoctorsFromFile(const string& filename) {
    vector<Doctor> doctors;
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << RED << "Warning: Could not open doctor file: " << filename << RESET << "\n";
        return doctors;
    }
    string line;
    while(getline(file,line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        Doctor d;
        getline(ss,d.name,'|');
        getline(ss,d.degree,'|');
        getline(ss,d.university,'|');
        getline(ss,d.age,'|');
        getline(ss,d.specialization,'|');
        getline(ss,d.department,'|');
        getline(ss,d.position,'|');
        getline(ss,d.experience,'|');
        getline(ss,d.additionalInfo,'|');
        doctors.push_back(d);
    }
    return doctors;
}

// Display doctor list
void displayDoctorList(const vector<Doctor>& doctors) {
    cout << "\n=== Allied Hospital Doctors ===\n";
    for(size_t i=0;i<doctors.size();i++)
        cout << i+1 << ". " << doctors[i].name << " (" << doctors[i].department << ")\n";
    cout << "0. Back to Menu\n";
}

// Display doctor details
void displayDoctorDetails(const Doctor& d) {
    cout << GREEN << "\n--- Doctor Details ---\n" << RESET;
    cout << "Name: " << d.name << "\n";
    cout << "Degree: " << d.degree << "\n";
    cout << "University: " << d.university << "\n";
    cout << "Age: " << d.age << "\n";
    cout << "Specialization: " << d.specialization << "\n";
    cout << "Department: " << d.department << "\n";
    cout << "Position: " << d.position << "\n";
    cout << "Experience: " << d.experience << "\n";
    cout << "Additional Info: " << d.additionalInfo << "\n";
}

// View doctors menu
void viewDoctorsMenu(const vector<Doctor>& doctors) {
    int choice;
    while(true) {
        displayDoctorList(doctors);
        cout << "\nSelect a doctor to view details (0 to go back): ";
        if(!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }
        if(choice==0) break;
        if(choice>0 && choice<=static_cast<int>(doctors.size()))
            displayDoctorDetails(doctors[choice-1]);
        else
            cout << RED << "Invalid choice! Try again.\n" << RESET;
    }
}

// Dijkstra shortest path (returns node id path, 1-based). totalDistance in meters.
vector<int> findShortestPath(int start,int end,double &totalDistance){
    int n = 8;
    if(start<1 || start>n || end<1 || end>n) { totalDistance = numeric_limits<double>::max(); return {}; }
    vector<double> distance(n+1,numeric_limits<double>::max());
    vector<int> prev(n+1,-1);
    distance[start] = 0;
    using PDI = pair<double,int>;
    priority_queue<PDI,vector<PDI>,greater<PDI>> pq;
    pq.push({0,start});
    while(!pq.empty()){
        auto [dist_u,u] = pq.top(); pq.pop();
        if(dist_u>distance[u]) continue;
        for(int v=1; v<=n; v++){
            if(dist[u][v] > 0) {
                double alt = distance[u] + dist[u][v];
                if(alt < distance[v]) {
                    distance[v] = alt;
                    prev[v] = u;
                    pq.push({alt, v});
                }
            }
        }
    }
    totalDistance = distance[end];
    if(totalDistance == numeric_limits<double>::max()) return {}; // no path
    vector<int> path;
    for(int at = end; at != -1; at = prev[at]) path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}

// Print path with colored arrows
void printPathWithArrows(const vector<int>& path){
    for(size_t i=0;i<path.size();i++){
        cout << GREEN << departments[path[i]-1].name << RESET;
        if(i+1 < path.size()) cout << CYAN << " ---> " << RESET;
    }
    cout << "\n";
}

// View Departments menu (Navigation)
void viewDepartmentsMenu(){
    while(true){
        cout << "\n=== Hospital Departments ===\n";
        for(auto &d : departments) cout << d.id << ". " << d.name << "\n";
        cout << "0. Back to Menu\n";

        int start,end;
        cout << "\nEnter Start Department ID: ";
        if(!(cin >> start)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }
        if(start==0) break;
        cout << "Enter End Department ID: ";
        if(!(cin >> end)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }

        double totalDistance = 0;
        vector<int> path = findShortestPath(start, end, totalDistance);

        if(path.empty()) {
            cout << RED << "No path exists between these departments!\n" << RESET;
            continue;
        }

        cout << "\nShortest Path:\n";
        printPathWithArrows(path);

        cout << fixed << setprecision(2);
        cout << "Total Distance: " << totalDistance << " meters (" << totalDistance/1000.0 << " km)\n";

        double normalSpeed = 1.4; // m/s ~ 5 km/h
        double fastSpeed = 2.0;   // m/s ~ 7.2 km/h
        double normalMin = (totalDistance / normalSpeed) / 60.0;
        double fastMin = (totalDistance / fastSpeed) / 60.0;
        cout << "Estimated Time: Normal walk: " << normalMin << " min, Fast walk: " << fastMin << " min\n";
    }
}

// ========================== LOGIN SYSTEM ==========================
bool loginPrompt() {
    while (true) {
        int roleChoice;
        cout << BLUE << "\n===== LOGIN PORTAL =====\n" << RESET;
        cout << "1. Admin\n2. User (Patient)\n3. Exit\n";
        cout << "Choose role: ";
        if(!(cin >> roleChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline

        if(roleChoice == 3) return false;

        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if(roleChoice == 1) {
            if(adminAccounts.count(username) && adminAccounts[username] == password) {
                isAdmin = true; isUser = false;
                cout << GREEN << "✅ Admin login successful!\n" << RESET;
                return true;
            } else {
                cout << RED << "❌ Invalid admin credentials! Try again.\n" << RESET;
                continue;
            }
        } else if(roleChoice == 2) {
            if(userAccounts.count(username) && userAccounts[username] == password) {
                isUser = true; isAdmin = false;
                cout << GREEN << "✅ User login successful!\n" << RESET;
                return true;
            } else {
                cout << RED << "❌ Invalid user credentials! Try again.\n" << RESET;
                continue;
            }
        } else {
            cout << RED << "Invalid option. Try again.\n" << RESET;
        }
    }
}

// ========================== MAIN ==========================
int main(){
    initializeDistances();
    vector<Doctor> doctors = loadDoctorsFromFile("private/doctors.txt");

    // Login
    if(!loginPrompt()) {
        cout << YELLOW << "Exiting...\n" << RESET;
        return 0;
    }

    while(true) {
        int mainChoice = -1;
        if(isAdmin) {
            cout << MAGENTA << "\n===== ADMIN PANEL =====\n" << RESET;
            cout << "1. View Departments\n";
            cout << "2. View Doctors\n";
            cout << "3. View Complaints (Coming soon)\n";
            cout << "4. Add Doctor (Coming soon)\n";
            cout << "5. Logout\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            if(!(cin >> mainChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }

            switch(mainChoice) {
                case 1: viewDepartmentsMenu(); break;
                case 2: viewDoctorsMenu(doctors); break;
                case 3: cout<<YELLOW<<"Viewing complaints (coming soon)\n"<<RESET; break;
                case 4: cout<<YELLOW<<"Add Doctor (coming soon)\n"<<RESET; break;
                case 5: // logout
                    isAdmin = false;
                    if(!loginPrompt()) { cout<<YELLOW<<"Exiting...\n"<<RESET; return 0; }
                    break;
                case 6: cout<<GREEN<<"Exiting... bye\n"<<RESET; return 0;
                default: cout<<RED<<"Invalid choice! Try again.\n"<<RESET;
            }
        }
        else if(isUser) {
            cout << BLUE << "\n===== USER MENU =====\n" << RESET;
            cout << "1. View Departments / Navigate\n";
            cout << "2. View Doctors\n";
            cout << "3. Register / Complain (Coming soon)\n";
            cout << "4. Logout\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            if(!(cin >> mainChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); cout<<RED<<"Invalid input\n"<<RESET; continue; }

            switch(mainChoice) {
                case 1: viewDepartmentsMenu(); break;
                case 2: viewDoctorsMenu(doctors); break;
                case 3: cout<<YELLOW<<"Register/Complain (coming soon)\n"<<RESET; break;
                case 4:
                    isUser = false;
                    if(!loginPrompt()) { cout<<YELLOW<<"Exiting...\n"<<RESET; return 0; }
                    break;
                case 5: cout<<GREEN<<"Exiting... bye\n"<<RESET; return 0;
                default: cout<<RED<<"Invalid choice! Try again.\n"<<RESET;
            }
        } else {
            // Not logged in (fall back to login)
            if(!loginPrompt()) { cout<<YELLOW<<"Exiting...\n"<<RESET; return 0; }
        }
    }

    return 0;
}
