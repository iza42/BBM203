# 🚁 Drone ALPHA Exploration System

The **Drone ALPHA Exploration System** is a C++-based project that simulates a drone’s journey across Turkish provinces. The drone explores provinces using a route planning system, which includes a stack-based backtracking mechanism and a queue-based system for managing the exploration of neighboring provinces. This system is designed to optimize exploration based on priorities and weather restrictions while adhering to maximum distance constraints.

## ⚙️ How It Works

The system models the journey of Drone ALPHA across various provinces, using different data structures to manage the exploration process. The key components are:

- **Stack Class**: Manages the backtracking of the drone's path, allowing it to return to previously visited provinces.
- **Queue Class**: Handles the exploration of neighboring provinces using a circular queue, with special priority given to neighboring provinces marked as priority areas.
- **Map Class**: Contains the distance data between provinces and provides functionalities to check if the distance between two provinces is within the allowable range.
- **RoutePlanner Class**: The core of the system, controlling the exploration logic, managing stacks and queues, handling constraints, and generating exploration results.

The drone will start at a specified province, explore neighboring provinces within distance constraints, backtrack when no further exploration is possible, and print results based on the journey.

## 🗂 Project Structure

DroneALPHA/
├── Stack.cpp # Implements stack-based backtracking for Drone ALPHA  
├── Stack.h # Header file for the Stack class  
├── Queue.cpp # Implements the circular queue for province exploration  
├── Queue.h # Header file for the Queue class  
├── Map.cpp # Manages the distance data and province visit status  
├── Map.h # Header file for the Map class  
├── RoutePlanner.cpp # Implements the route planning and exploration logic  
├── RoutePlanner.h # Header file for the RoutePlanner class  
├── main.cpp # Entry point to the program and initiates the exploration  
├── distance_data.csv # Input file containing the distance matrix between provinces  
├── priority_provinces.txt # File containing the list of priority provinces  
├── weather_restricted_provinces.txt # File containing the list of weather-restricted provinces  
└── README.md # Project description and usage guide

## ▶️ How to Use

1. **Compile the C++ files**:
   g++ -std=c++11 _.cpp _.h -o DroneALPHA
2. **Run the program with the required arguments**:
   ./DroneALPHA max_distance starting_city_index distance_file priority_provinces_file weather_restricted_provinces_file

   max_distance: The maximum distance the drone can travel in a single move.
   starting_city_index: The index of the starting city for the exploration.
   distance_file: The CSV file containing the distance data between provinces.
   priority_provinces_file: The file listing the priority provinces for exploration.
   weather_restricted_provinces_file: The file listing the weather-restricted provinces.

## 📝 Additional Notes

The program uses a static array-based stack and queue for efficient management of the drone's exploration path.

The maximum distance for exploration is constrained by the max_distance parameter, and only neighboring provinces within the distance limit will be explored.

Priority provinces are explored before others, and weather-restricted provinces are avoided during exploration.
