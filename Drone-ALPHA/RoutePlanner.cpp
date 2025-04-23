#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    std::fill_n(priorityProvinces, MAX_PRIORITY_PROVINCES, -2); // used -2 cuz I used -1 for other controls
    std::fill_n(visitedPriorityProvinces, MAX_PRIORITY_PROVINCES, -2);
    std::fill_n(weatherRestrictedProvinces, MAX_WEATHER_RESTRICTED_PROVINCES, -2);
    std::fill_n(skippedWeatherRestrictedProvinces, MAX_WEATHER_RESTRICTED_PROVINCES, -2);

    map.loadDistanceData(distance_data);   // Load map data from file
    map.resetVisited();   // Mark all provinces as unvisited initially
    loadPriorityProvinces(priority_data);    // Load priority provinces
    loadWeatherRestrictedProvinces(restricted_data);    // Load weather restricted provinces
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    for (int i = 0; i < 10; ++i) {
        this->priorityProvinces[i] = -1; // making all the elements -1 for avoiding garbage values
    }
    std::ifstream file(filename); // opening the file
    if (!file.is_open()) {
        std::cerr << "Cannot open the priority provinces file." << std::endl;
        return;
    }
    std::string sentence;
    while(std::getline(file, sentence)) {
        if(!sentence.empty()) {
            if (sentence.back() == '\r') { // this solved the problem about printing the values into the screen
                sentence.pop_back(); // popped the extra \r value
            }
            size_t start = sentence.find('(');
            size_t end = sentence.find(')');
            std::string number = sentence.substr(start + 1, end - start - 1);
            this->priorityProvinces[numPriorityProvinces++]=stoi(number);
        }

    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    for (int &a: this->weatherRestrictedProvinces) { //making it -1 for avoiding garbage values
        a =-1;
    }
    std::fstream file (filename);
    if(!file.is_open()) {
        std::cerr<<"weather restricted dosyasını açamıyorum"<<std::endl;
        return ;
    }
    std::string sentence;
    while (getline(file, sentence)) {
        if(!sentence.empty()) {
            if (sentence.back() == '\r') { // this solved the problem about printing the values into the screen
                sentence.pop_back(); // removed the extra \r value
            }
            size_t start = sentence.find('(');
            size_t end = sentence.find(')');
            std::string number = sentence.substr(start + 1, end - start - 1);
            this->weatherRestrictedProvinces[numWeatherRestrictedProvinces++]=stoi(number);
        }

    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int a : priorityProvinces) {
        if(a==province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int a : weatherRestrictedProvinces) {
        if(a==province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
// set the starting city
    stack.push(startingCity);
    map.markAsVisited(startingCity);
    route.push_back(startingCity);

    // add starting cities neighbor's to the queue
    enqueueNeighbors(startingCity);

    int previousProvince = startingCity; // for the distance calculation

    // the main exploration loop
    while (!isExplorationComplete()) {
        if (!queue.isEmpty()) {
            int currentProvince = queue.dequeue(); // take the province for visiting

            if (!map.isVisited(currentProvince)) {
                map.markAsVisited(currentProvince);
                stack.push(currentProvince);
                route.push_back(currentProvince);

                if(isPriorityProvince(currentProvince)) {
                    visitedPriorityProvinces[visited_priority_provinces_counter++]=currentProvince;
                }
                totalDistanceCovered += map.getDistance(previousProvince, currentProvince);

                previousProvince = currentProvince;
                enqueueNeighbors(currentProvince);
            } else {
               // do nothing, cuz we do nothing for already visited provinces
            }
        } else {
            backtrack(); // if queue is empty, backtrack
        }
    }
    displayResults();

    // clearing the memory
    route.clear();
    while (!queue.isEmpty()) {
        queue.dequeue();
    }
    while (!stack.isEmpty()) {
        stack.pop();
    }

}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    //if exploration will start from zero, all data's must be reset.
    map.resetVisited();
    route.clear();
    totalDistanceCovered = 0;

    // clear queue and stack
    while (!queue.isEmpty()) {
        queue.dequeue();
    }
    while (!stack.isEmpty()) {
        stack.pop();
    }

    exploreRoute(province); // start the exploration
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int neighbor = 0; neighbor < MAX_QUEUE_SIZE; neighbor++) { //
// if the province out of the distance range skip
        if (!(map.distanceMatrix[province][neighbor] <= maxDistance &&
      map.distanceMatrix[province][neighbor] > 0)) {
            continue;
        }

        // if the neighbor already visited, skip
        if (map.isVisited(neighbor)) {
            continue;
        }

        // if the province is weather restricted , skip
        if (isWeatherRestricted(neighbor)) {
           skippedWeatherRestrictedProvinces[weatherestrictedcounter++]=neighbor;
            continue;
        }

        // if it is priority province use enqueue priority for this
        if (isPriorityProvince(neighbor)) {
            queue.enqueuePriority(neighbor);
        } else {
            queue.enqueue(neighbor);
        }
    }
}

void RoutePlanner::backtrack() {
    if (isExplorationComplete()) {
        return;
    }
    // If you reach a dead-end province
    // no province for backtracking, the stack is empty
    if (stack.isEmpty()) {
        return;
    }
 // backtracking, taking the last visited city
    int previousProvince = stack.pop();
    if (previousProvince == -1) {
        // if the stack is empty ,so I get -1 instead of a province
        return;
    }

    // if the queue empty, add
    if (queue.isEmpty()) {
        queue.enqueue(previousProvince);

    }else {
       // do not what to do here
    }
}

bool RoutePlanner::isExplorationComplete() const {
    // checking  if stack and queue is empty at the same time
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    // Weather-Restricted Provinces
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; ++i) {
        if (skippedWeatherRestrictedProvinces[i] != -2) {
            // Duplicate check
            bool isDuplicate = false;
            for (int j = 0; j < i; ++j) {
                if (skippedWeatherRestrictedProvinces[j] == skippedWeatherRestrictedProvinces[i]) {
                    isDuplicate = true;
                    break;
                }
            }
            // if not duplicate , print it.
            if (!isDuplicate) {
                int province = skippedWeatherRestrictedProvinces[i];
                std::cout << "Province " << cities[province] << " is weather-restricted. Skipping." << std::endl;
            }
        }
     }

    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;

    // Total Provinces Visited
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;

    // Total Distance Covered
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;

    // Route Taken
    std::cout << "Route Taken:" << std::endl;
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i != route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    //  Priority Provinces Status
    std::cout << "Priority Provinces Status:" << std::endl;
    int visitedCount = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        bool visited = false;

        for (int j = 0; j < MAX_PRIORITY_PROVINCES; ++j) {
            if (visitedPriorityProvinces[j] == province) {
                visited = true;
                break;
            }
        }

        if (visited) visitedCount++;
        std::cout << "- " << cities[province] << " (" << (visited ? "Visited" : "Not Visited") << ")" << std::endl;
    }
    //  Priority Provinces Summary
    std::cout << "Total Priority Provinces Visited: " << visitedCount << " out of " << numPriorityProvinces << std::endl;

    if (visitedCount == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    } else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}


