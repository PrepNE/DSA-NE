#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>

using namespace std;

struct City {
    int index;
    string name;
};

struct Road {
    string city1;
    string city2;
    float budget;
};

vector<City> cities;
vector<Road> roads;

const int MAX_CITIES = 100;

int adjacencyMatrix[MAX_CITIES][MAX_CITIES] = {0};
long long budgetMatrix[MAX_CITIES][MAX_CITIES] = {0};

// Function declarations
void initializeSystem();
void addNewCities();
void addNewRoads();
void assignBudget();
void editCity();
void searchCity();
void saveToFiles();
void displayCities();
void displayRoads();
void displayAllData();
void displayMainMenu();
void loadInitialData();
string getCityName(int index);
int findCityIndex(const string &name);
bool cityExists(const string &name);
bool roadExists(const string &city1, const string &city2);

// Input validation helper functions
int getValidInteger(const string& prompt, int minValue = INT_MIN, int maxValue = INT_MAX);
long long getValidLongLong(const string& prompt, long long minValue = 0);
void clearInputBuffer();

int main() {
    initializeSystem();
    loadInitialData();

    int choice;

    do {
        displayMainMenu();
        choice = getValidInteger("Enter your choice (1-9): ", 1, 9);

        switch(choice) {
            case 1: addNewCities(); break;
            case 2: addNewRoads(); break;
            case 3: assignBudget(); break;
            case 4: editCity(); break;
            case 5: searchCity(); break;
            case 6: displayCities(); break;
            case 7: displayRoads(); break;
            case 8: displayAllData(); break;
            case 9:
                saveToFiles();
                cout << "Data saved to files. Exiting program.\n";
                break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 9);

    return 0;
}

// Input validation helper functions
int getValidInteger(const string& prompt, int minValue, int maxValue) {
    string input;
    int value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Check if input is empty
        if (input.empty()) {
            cout << "Error: Please enter a value.\n";
            continue;
        }
        
        // Try to convert string to integer
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            // Check if value is within valid range
            if (value >= minValue && value <= maxValue) {
                return value;
            } else {
                cout << "Error: Please enter a number between " << minValue << " and " << maxValue << ".\n";
            }
        } else {
            cout << "Error: Please enter a valid number.\n";
        }
    }
}

long long getValidLongLong(const string& prompt, long long minValue) {
    string input;
    long long value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Check if input is empty
        if (input.empty()) {
            cout << "Error: Please enter a value.\n";
            continue;
        }
        
        // Try to convert string to long long
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            // Check if value is within valid range
            if (value >= minValue) {
                return value;
            } else {
                cout << "Error: Please enter a number greater than or equal to " << minValue << ".\n";
            }
        } else {
            cout << "Error: Please enter a valid number.\n";
        }
    }
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void initializeSystem() {
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            adjacencyMatrix[i][j] = 0;
            budgetMatrix[i][j] = 0;
        }
    }
}

void loadInitialData() {
    cities.push_back({1, "Kigali"});
    cities.push_back({2, "Huye"});
    cities.push_back({3, "Muhanga"});
    cities.push_back({4, "Musanze"});
    cities.push_back({5, "Nyagatare"});
    cities.push_back({6, "Rubavu"});
    cities.push_back({7, "Rusizi"});

    roads.push_back({"Kigali", "Muhanga", 28.6});
    roads.push_back({"Kigali", "Musanze", 28.6});
    roads.push_back({"Kigali", "Nyagatare", 70.84});
    roads.push_back({"Muhanga", "Huye", 56.7});
    roads.push_back({"Musanze", "Rubavu", 33.7});
    roads.push_back({"Huye", "Rusizi", 80.96});
    roads.push_back({"Muhanga", "Rusizi", 117.5});
    roads.push_back({"Musanze", "Nyagatare", 96.14});
    roads.push_back({"Muhanga", "Nyagatare", 66.3});

    // Update adjacency and budget matrices
    for(const auto& road : roads) {
        int idx1 = findCityIndex(road.city1);
        int idx2 = findCityIndex(road.city2);
        if(idx1 != -1 && idx2 != -1) {
            adjacencyMatrix[idx1][idx2] = 1;
            adjacencyMatrix[idx2][idx1] = 1;
            budgetMatrix[idx1][idx2] = road.budget;
            budgetMatrix[idx2][idx1] = road.budget;
        }
    }
}

void displayMainMenu() {
    cout << "\n=== Rwanda Infrastructure Management System ===\n";
    cout << "1. Add New Cities\n";
    cout << "2. Add Roads Between Cities\n";
    cout << "3. Assign Road Budgets\n";
    cout << "4. Edit City\n";
    cout << "5. Search for City\n";
    cout << "6. Display Cities\n";
    cout << "7. Display Roads\n";
    cout << "8. Display All Recorded Data\n";
    cout << "9. Exit\n";
}

void addNewCities() {
    int numberOfCities = getValidInteger("\nHow many new cities do you want to add? ", 1, 50);

    for (int i = 0; i < numberOfCities; i++) {
        string cityName;
        cout << "Enter Name of City " << (i + 1) << ": ";
        getline(cin, cityName);

        // Check for empty input
        if (cityName.empty()) {
            cout << "Error: City name cannot be empty. Skipping...\n";
            i--; // Retry this iteration
            continue;
        }

        // Check if city already exists
        if (cityExists(cityName)) {
            cout << "City '" << cityName << "' already exists. Skipping...\n";
            continue;
        }

        City newCity;
        newCity.index = cities.empty() ? 1 : cities.back().index + 1;
        newCity.name = cityName;

        cities.push_back(newCity);
        cout << "City '" << newCity.name << "' added with ID: #" << newCity.index << endl;
    }
}

void addNewRoads() {
    string city1, city2;

    cout << "\nEnter first city name or index: ";
    getline(cin, city1);
    
    // Check for empty input
    if (city1.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }
    
    cout << "Enter second city name or index: ";
    getline(cin, city2);
    
    // Check for empty input
    if (city2.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }

    int idx1 = findCityIndex(city1);
    int idx2 = findCityIndex(city2);

    if (idx1 == -1 || idx2 == -1) {
        cout << "One or both cities not found.\n";
        return;
    }

    if (idx1 == idx2) {
        cout << "Cannot create a road from a city to itself.\n";
        return;
    }

    city1 = getCityName(idx1);
    city2 = getCityName(idx2);

    bool roadCurrentlyExists = roadExists(city1, city2);

    if (roadCurrentlyExists) {
        cout << "A road already exists between " << city1 << " and " << city2 << ".\n";
        cout << "Do you want to remove this road? (1 for yes, 0 for no): ";
    } else {
        cout << "No road currently exists between " << city1 << " and " << city2 << ".\n";
        cout << "Do you want to add a road? (1 for yes, 0 for no): ";
    }

    int choice = getValidInteger("", 0, 1);

    if (choice == 1) {
        if (roadCurrentlyExists) {
            // Remove the road
            adjacencyMatrix[idx1][idx2] = 0;
            adjacencyMatrix[idx2][idx1] = 0;
            budgetMatrix[idx1][idx2] = 0;
            budgetMatrix[idx2][idx1] = 0;

            for (auto it = roads.begin(); it != roads.end();) {
                if ((it->city1 == city1 && it->city2 == city2) ||
                    (it->city1 == city2 && it->city2 == city1)) {
                    it = roads.erase(it);
                    break;
                } else {
                    ++it;
                }
            }
            cout << "Road between " << city1 << " and " << city2 << " removed.\n";
        } else {
            // Add the road
            adjacencyMatrix[idx1][idx2] = 1;
            adjacencyMatrix[idx2][idx1] = 1;
            roads.push_back({city1, city2, 0});
            cout << "Road added between " << city1 << " and " << city2 << ".\n";
        }
    } else {
        cout << "No changes made.\n";
    }
}

void assignBudget() {
    string city1, city2;

    cout << "\nEnter first city name or index: ";
    getline(cin, city1);
    
    // Check for empty input
    if (city1.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }
    
    cout << "Enter second city name or index: ";
    getline(cin, city2);
    
    // Check for empty input
    if (city2.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }

    int idx1 = findCityIndex(city1);
    int idx2 = findCityIndex(city2);

    if(idx1 == -1 || idx2 == -1) {
        cout << "One or both cities not found.\n";
        return;
    }

    // Check if road exists
    if(adjacencyMatrix[idx1][idx2] == 0) {
        cout << "No road exists between these cities. Cannot assign budget.\n";
        cout << "Please add a road first using option 2.\n";
        return;
    }

    city1 = getCityName(idx1);
    city2 = getCityName(idx2);

    cout << "Current budget: " << budgetMatrix[idx1][idx2] << " RWF\n";
    
    long long budget = getValidLongLong("Enter new budget for road between " + city1 + " and " + city2 + " (in RWF): ", 0);

    budgetMatrix[idx1][idx2] = budget;
    budgetMatrix[idx2][idx1] = budget;

    // Update the budget in the roads vector
    for (auto& road: roads) {
        if ((road.city1 == city1 && road.city2 == city2) ||
            (road.city1 == city2 && road.city2 == city1)) {
            road.budget = budget;
            break;
        }
    }
    cout << "Budget of " << budget << " RWF assigned successfully.\n";
}

void editCity() {
    string input;
    cout << "\nEnter city index or name to edit: ";
    getline(cin, input);
    
    // Check for empty input
    if (input.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }

    int cityIndex = -1;

    // Check if input is a number
    if (!input.empty() && isdigit(input[0])) {
        stringstream ss(input);
        int index;
        if (ss >> index && ss.eof()) {
            for (const auto &city: cities) {
                if (city.index == index) {
                    cityIndex = city.index;
                    break;
                }
            }
        }
    } else {
        // Search by name
        for (const auto &city: cities) {
            if (city.name == input) {
                cityIndex = city.index;
                break;
            }
        }
    }

    if(cityIndex == -1) {
        cout << "City not found.\n";
        return;
    }

    auto it = find_if(cities.begin(), cities.end(), [cityIndex](const City &city) {
        return city.index == cityIndex;
    });

    if (it == cities.end()) {
        cout << "City not found.\n";
        return;
    }

    string oldName = it->name;
    cout << "Current name: " << oldName << endl;
    cout << "Enter new name: ";
    string newName;
    getline(cin, newName);
    
    // Check for empty input
    if (newName.empty()) {
        cout << "Error: City name cannot be empty.\n";
        return;
    }

    // Check if new name already exists (and it's not the same city)
    if (cityExists(newName) && newName != oldName) {
        cout << "A city with name '" << newName << "' already exists.\n";
        return;
    }

    // Update city name in roads vector
    for (auto& road: roads) {
        if (road.city1 == oldName) road.city1 = newName;
        if (road.city2 == oldName) road.city2 = newName;
    }

    it->name = newName;
    cout << "City updated successfully from '" << oldName << "' to '" << newName << "'.\n";
}

void searchCity() {
    string input;
    cout << "\nEnter city index or name to search: ";
    getline(cin, input);
    
    // Check for empty input
    if (input.empty()) {
        cout << "Error: City name/index cannot be empty.\n";
        return;
    }

    City* foundCity = nullptr;

    if(!input.empty() && isdigit(input[0])) {
        stringstream ss(input);
        int index;
        if (ss >> index && ss.eof()) {
            for(auto& city : cities) {
                if(city.index == index) {
                    foundCity = &city;
                    break;
                }
            }
        }
    } else {
        for(auto& city : cities) {
            if(city.name == input) {
                foundCity = &city;
                break;
            }
        }
    }

    if(foundCity == nullptr) {
        cout << "City not found.\n";
        return;
    }

    cout << "City found:\n";
    cout << "Index: " << foundCity->index << "\nName: " << foundCity->name << endl;

    // Display connected cities
    cout << "Connected to: ";
    bool hasConnections = false;
    int cityIdx = findCityIndex(foundCity->name);

    for(int i = 0; i < cities.size(); i++) {
        if(adjacencyMatrix[cityIdx][i] == 1) {
            cout << getCityName(i);
            if(budgetMatrix[cityIdx][i] > 0) {
                cout << " (Budget: " << budgetMatrix[cityIdx][i] << " RWF)";
            }
            cout << ", ";
            hasConnections = true;
        }
    }

    if(!hasConnections) {
        cout << "None";
    } else {
        cout << "\b\b "; // Remove last comma and space
    }
    cout << endl;
}

void saveToFiles() {
    // Save cities
    ofstream cityFile("cities.txt");
    if (cityFile.is_open()) {
        cityFile << left << setw(10) << "Index" << setw(20) << "City_Name" << endl;
        cityFile << string(30, '-') << endl;
        for(const auto& city : cities) {
            cityFile << left << setw(10) << city.index << setw(20) << city.name << endl;
        }
        cityFile.close();
        cout << "Cities data saved to 'cities.txt' successfully.\n";
    } else {
        cout << "Error: Could not save cities data to file.\n";
    }

    // Save roads
    ofstream roadFile("roads.txt");
    if(roadFile.is_open()) {
        roadFile << left << setw(5) << "Nbr" << setw(30) << "Road" << "Budget (Billion RWF)" << endl;
        roadFile << string(60, '-') << endl;
        for(int i = 0; i < roads.size(); i++) {
            roadFile << left << setw(5) << i+1
                     << setw(30) << (roads[i].city1 + " - " + roads[i].city2)
                     << roads[i].budget << endl;
        }
        roadFile.close();
        cout << "Roads data saved to 'roads.txt' successfully.\n";
    } else {
        cout << "Error: Could not save roads data to file.\n";
    }
}

void displayAllData() {
    displayCities();
    displayRoads();
}

void displayCities() {
    if (cities.empty()) {
        cout << "\nNo cities recorded.\n";
        return;
    }
    cout << "\nList of Cities:\n";
    cout << left << setw(10) << "Index" << setw(20) << "City Name" << endl;
    cout << string(30, '-') << endl;

    for (const auto &city: cities) {
        cout << left << setw(10) << city.index << setw(20) << city.name << endl;
    }
}

void displayRoads() {
    if(roads.empty()) {
        cout << "\nNo roads recorded.\n";
        return;
    }

    cout << "\nList of Roads and Budgets:\n";
    cout << left << setw(5) << "Nbr" << setw(30) << "Road" << "Budget (RWF)" << endl;
    cout << string(60, '-') << endl;

    for(int i = 0; i < roads.size(); i++) {
        cout << left << setw(5) << i+1
             << setw(30) << (roads[i].city1 + " - " + roads[i].city2)
             << roads[i].budget << endl;
    }

    if(cities.size() > 0) {
        cout << "\nRoad Connection Matrix (1 = road exists, 0 = no road):\n";
        cout << "     ";
        for(int i = 0; i < cities.size(); i++) {
            cout << setw(3) << cities[i].index;
        }
        cout << endl;

        for(int i = 0; i < cities.size(); i++) {
            cout << setw(3) << cities[i].index << ": ";
            for(int j = 0; j < cities.size(); j++) {
                cout << setw(3) << adjacencyMatrix[i][j];
            }
            cout << endl;
        }

        // Display budget matrix
        cout << "\nRoad Budget Matrix (in RWF):\n";
        cout << "     ";
        for(int i = 0; i < cities.size(); i++) {
            cout << setw(12) << cities[i].index;
        }
        cout << endl;

        for(int i = 0; i < cities.size(); i++) {
            cout << setw(3) << cities[i].index << ": ";
            for(int j = 0; j < cities.size(); j++) {
                cout << setw(12) << budgetMatrix[i][j];
            }
            cout << endl;
        }
    }
}

// Utility Functions
int findCityIndex(const string &name) {
    // Check if input is a number
    if (!name.empty() && isdigit(name[0])) {
        stringstream ss(name);
        int index;
        if (ss >> index && ss.eof()) {
            for (int i = 0; i < cities.size(); i++) {
                if (cities[i].index == index) {
                    return i; // Return position in vector, not the city index
                }
            }
        }
    } else {
        // Search by name
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i].name == name) {
                return i; // Return position in vector
            }
        }
    }
    return -1;
}

string getCityName(int vectorIndex) {
    if (vectorIndex >= 0 && vectorIndex < cities.size()) {
        return cities[vectorIndex].name;
    }
    return "";
}

bool cityExists(const string &name) {
    for (const auto &city : cities) {
        if (city.name == name) {
            return true;
        }
    }
    return false;
}

bool roadExists(const string &city1, const string &city2) {
    for (const auto &road : roads) {
        if ((road.city1 == city1 && road.city2 == city2) ||
            (road.city1 == city2 && road.city2 == city1)) {
            return true;
        }
    }
    return false;
}