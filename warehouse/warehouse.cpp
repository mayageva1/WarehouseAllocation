//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - warehouse.cpp
//

#include "warehouse.h"

//
// CLASS: Warehouse
// This class represents a typical warehouse environment. Each instance of Warehouse contains a 2D vector of StorageUnit
// instances, a 2D vector of GraphEdge instances to represent traversing the Warehouse space, and a RangeTree instance
// representing the remaining available space within the Warehouse instance's StorageUnits. Functions of the Warehouse
// class include adding new StorageUnit and Item instances, finding specific items within the Warehouse, and finding the
// shortest path between either individual storage units or a series of items. This class employs the three required
// data structures and algorithms - Dijkstra's Algorithm, Fractional Knapsack, and Range Tree - to fulfill its objective.
// The class also stores general usage statistics to be exported at the conclusion of the program.
//

// CLASS INSTANTIATION: Creating a static instance of the Algorithms class. The required algorithms are contained within
// this class. This instance is how the algorithms are accessed by the Warehouse instance.
static Algorithms alg;

// CONSTRUCTOR: Default constructor for the Warehouse class. Resizes the 2D vector of StorageUnit instances to a 1x1
// grid so that StorageUnit instances can be added to the Warehouse. All other member variables are either declared in
// the header file or at a later time.
Warehouse::Warehouse(){
    units.resize(1, std::vector<StorageUnit>(1));
}

// CONSTRUCTOR: Creates a Warehouse instance with StorageUnit instances at the time of creation. Accepts parameter
// units_in, a vector of StorageUnit instances to add to the Warehouse.
Warehouse::Warehouse(std::vector<StorageUnit> units_in){
    units.resize(1, std::vector<StorageUnit>(1));
    for(StorageUnit i : units_in) add_unit(i);
}

// FUNCTION: Adds a new StorageUnit instance with a specified capacity but no coordinates to the Warehouse. The new StorageUnit
// instance is created with the given capacity and the coordinates of the first empty space in the Warehouse. Accepts
// parameter capacity, an integer representing the size of the StorageUnit.
void Warehouse::add_unit(int capacity){
    bool added = false;
    for(int i = 0; i < units.size(); i++){
        for(int j = 0; j < units[i].size(); j++){
            // Find the first empty space in the Warehouse and add a new StorageUnit instance.
            if(!added && units[i][j].getCapacity() == 0){
                add_unit(StorageUnit(capacity, {i,j}));
                added = true;
            }
        }
    }
    // If there was no empty space in the Warehouse, the new StorageUnit will be placed at a new space in the XCoord direction.
    // By calling add_unit(StorageUnit ...) outside of the Warehouse instance's bounds, the function will resize the Warehouse.
    if(!added) add_unit(StorageUnit(capacity, {units.size(), units.size() - 1}));
    return;
}

// FUNCTION: Adds a new StorageUnit instance to the Warehouse. Checks if the Warehouse needs to be resized with the addition
// of the new StorageUnit. If so, this function resizes the Warehouse and copies all previous data. Accepts parameter unit,
// an instance of StorageUnit.
void Warehouse::add_unit(StorageUnit unit){
    // Checking if the Warehouse is large enough to store the new StorageUnit at its given location.
    int newSize = 0;
    int new_unit_max = std::max(unit.getLocation().first, unit.getLocation().second);
    for(std::vector<StorageUnit> i : units){
        for(StorageUnit u : i){
            // Checking what is bigger, the current Warehouse dimensions or the position of the StorageUnit to be inserted;
            int existing_unit_max = std::max(u.getLocation().first, u.getLocation().second);
            newSize = std::max(newSize, std::max(new_unit_max, existing_unit_max));
        }
    }

    // Adjustment for C++ index starting at 0.
    newSize++;

    // If the new StorageUnit is outside the bounds of the Warehouse, the Warehouse needs to be resized.
    if(newSize > units.size()){
        std::vector<std::vector<StorageUnit> > temp(newSize, std::vector<StorageUnit>(newSize));

        for(int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
                // Filling the new Warehouse StorageUnit 2D vector.
                temp[i][j] = StorageUnit(0, {i, j});
            }
        }
        for(int i = 0; i < units.size(); i++){
            for(int j = 0; j < units[i].size(); j++){
                // Copying the data from the old 2D vector.
                if(units[i][j].getCapacity() != 0) temp[i][j] = units[i][j];
            }
        }
        units = temp;
    }

    // Counter for the number of StorageUnit instances in the Warehouse.
    num_units++;
    // Adding the StorageUnit's capacity to the total Warehouse capacity.
    capacity += unit.getCapacity();
    // If a StorageUnit already has items in it, add the capacity of those items to the Warehouse's used capacity counter.
    used_capacity += unit.getUsedCapacity();
    // Assigning the new StorageUnit to the 2D vector in the Warehouse instance.
    units[unit.getLocation().first][unit.getLocation().second] = unit;
    // Updating the Range Tree with the new StorageUnit.
    tree.insert(unit);
    // Updating the Adjacency List with the new StorageUnit.
    graph = alg.buildGraph(units);
    return;
}

// FUNCTION: Helper function used to sort a vector of StorageUnit instances. Compares each StorageUnit by their remaining
// available space. Accepts two StorageUnit pointers as parameters and returns a boolean.
bool compare(StorageUnit& a, StorageUnit& b) {
    return (a.getCapacity() - a.getUsedCapacity()) < (b.getCapacity() - b.getUsedCapacity());
}

// FUNCTION: Adds a new Item instance to the Warehouse. The function attempts to find a StorageUnit instance within the
// Warehouse that can accommodate the entire Item instance, however, if that is not possible, it uses the Fractional Knapsack
// algorithm to distribute the Item across multiple StorageUnits. Parameter is Item i, an instance of Item.
void Warehouse::add(Item i){
    // Perform a range query on the range tree to find StorageUnit instances that can accommodate the Item. The lower
    // bound of the range query is the size of a single Item and the upper bound is the size of the item multiplied
    // by the quantity to represent the total amount of space the Item instance consumes.
    std::vector<StorageUnit> possible_units = tree.rangeQuery({i.size_per_unit, i.size_per_unit * i.quantity});
    // If there are no StorageUnits that can accommodate the entire Item instance, the Item is passed to the fractional
    // knapsack algorithm.
    if(possible_units.empty()){
        // Fractional knapsack returns an integer representing the amount of space it was able to use and a vector of
        // coordinates representing StorageUnit instances that had partial Items added.
        std::pair<int, std::vector<std::pair<int, int> > > results = alg.fknapsack(units, i);
        int addl_used = results.first;
        // If the space used by the fractional knapsack algorithm is not the equal to the entire Item's space, notify
        // the user that not all the Item was able to fit in the Warehouse.
        if(addl_used != (i.size_per_unit * i.quantity)) std::cout << "[Add Error] Unable to store " <<  (i.size_per_unit * i.quantity) - addl_used << " " << i.name << "(s) due to lack of available storage space." << std::endl;
        // Adding the space consumed by the new Item to the Warehouse's counter.
        used_capacity += addl_used;
        // For every StorageUnit that was modified, update the range tree to reflect the changes.
        for(std::pair<int, int> loc : results.second) {
            tree.updateNode(loc, units[loc.first][loc.second]);
        }
    } else {
        // If there is an entire StorageUnit that can accommodate the new Item instance, add the entire Item to the
        // StorageUnit.
        // Adding the Item to the most ideal StorageUnit instance.
        std::sort(possible_units.begin(), possible_units.end(), compare);
        std::pair<int,int> loc = possible_units[0].getLocation();
        units[loc.first][loc.second].add(i);
        // Adding the space consumed by the new Item to the Warehouse's counter.
        used_capacity += (i.size_per_unit * i.quantity);
        // Updating the range tree to reflect the changes.
        tree.updateNode(loc, units[loc.first][loc.second]);
    }
    return;
}

// FUNCTION: Returns the max capacity of the Warehouse instance.
int Warehouse::getSize() {
    return this->capacity;
}

// FUNCTION: Returns the used capacity of the Warehouse instance.
int Warehouse::getUsage() {
    return this->used_capacity;
}

// FUNCTION: Returns the percentage of the Warehouse instance that is in use.
float Warehouse::getUsagePercentage() {
    return 100.0 * ((float)used_capacity/capacity);
}

// FUNCTION: Returns a StorageUnit instance from inside the Warehouse. Accepts parameter loc, a pair of integers representing
// coordinates.
StorageUnit Warehouse::getUnit(std::pair<int, int> loc) {
    return units[loc.first][loc.second];
}

// FUNCTION: Locates any Item instance in the Warehouse that matches the provided name. Parameter i_name is a string representing\
// the name of the Item to find. Returns a vector of integers, representing coordinates for all StorageUnit instances that\
// contain the Item.
std::vector<std::pair<int, int> > Warehouse::findItem(std::string i_name) {
    std::vector<std::pair<int, int> > found_locations;
    // Iterate over each unit in the Warehouse.
    for(int i = 0; i < units.size(); i++){
        for(int j = 0; j < units[i].size(); j++){
            // Retrieving the Item map of the StorageUnit and checking if the Item exists.
            std::map<std::string, Item> items = units[i][j].getItems();
            if(items.find(i_name) != items.end()) found_locations.push_back({i,j});
        }
    }
    return found_locations;
}

// FUNCTION: Calculates the shortest path between an origin point and a series of destination locations. Utilizes
// Dijkstra's Algorithm to complete this calculation. Accepts parameters src, a pair of integers representing the starting
// coordinates, and dest, a vector of integer pairs representing the locations to travel to from the source node.
// Returns an integer representing the total distance between the source and destination(s). The function also prints the
// shortest path to the standard output.
int Warehouse::getPath(std::pair<int, int> src, std::vector <std::pair<int, int>> dest) {
    int totaldistance = 0;
    std::vector<std::pair<int, int> > path;

    std::pair<int, int> source;

    // Iterate through each destination
    for(std::pair<int, int> coords : dest){
        if(path.empty()) source = src;
        else source = path[path.size() - 1];
        // Using Dijkstra's Algorithm to find the shortest path from each StorageUnit instance to the next.
        std::pair<int, std::vector<std::pair<int, int> > > results = alg.dijkstra(units, graph, source, coords);
        // Add the distance for each specific traversal to the total for the entire traversal.
        totaldistance += results.first;
        // Push the path traveled to the vector tracking the shortest path.
        for(std::pair<int, int> path_traversal : results.second){
            path.push_back(path_traversal);
        }
    }

    // Print the shortest path information to the standard output.
    std::cout << "[FIND_PATH_UNITS] Shortest path between (" << path[0].first << "," << path[0].second << ") and (" << path[path.size() - 1].first << "," << path[path.size() - 1].second << "): " << totaldistance << " units\nPath: ";
    for (std::pair<int, int> node : path) {
        std::cout << "(" << node.first << "," << node.second << ") ";
    }
    std::cout << std::endl;

    return totaldistance;
}

// FUNCTION: Calculates the shortest path between a specified origin point and a series of Items within the Warehouse.
// Accepts parameters src, a pair of integers representing the starting coordinates, and items, a vector of strings representing
// the names of Items to find and travel to. For this function, if an Item is present in multiple StorageUnits, a path is
// only calculated for the first. Returns an integer representing the total distance between the source and destination(s).
// The function also prints the shortest path to the standard output.
int Warehouse::getPath(std::pair<int, int> src, std::vector<std::string> items) {
    std::vector<std::pair<int, int> > path;

    // Iterates over all Items to be traveled to.
    for(std::string i : items){
        std::cout << "[FIND_PATH_ITEMS] Shortest path from current location to " << i <<std::endl;
        // Finds the location of the current item and pushes the location to a vector of integers representing coordinates
        // if the item can be found. If an item cannot be found, the function returns -1 as no path can be created through
        // a nonexistent StorageUnit.
        std::vector<std::pair<int, int> > loc = findItem(i);
        if(!loc.empty() && loc[0].first != -1) path.push_back(loc[0]);
        else{
            std::cout << "[FIND_PATH_ITEMS] One of the items could not be found." << std::endl;
            return -1;
        }
    }

    // Calculating the total distance of the path between the items and printing to the standard output.
    int totaldistance = getPath(src, path);
    std::cout << "[FIND_PATH_ITEMS] Shortest path between all items: " << totaldistance << " units" << std::endl;

    return totaldistance;
}

// FUNCTION: Generates and exports various statistics, visualizations, and data for the current Warehouse instance.
void Warehouse::print() {
    //
    // Exporting Warehouse Statistics
    //

    std::ofstream stat_out_file("./exports/warehouse_statistics.txt");

    stat_out_file << "Warehouse {" << std::endl;
    stat_out_file << std::endl;

    stat_out_file << "\tStatistics {" << std::endl;
    stat_out_file << "\t\tNumber of Units: " << num_units << std::endl;
    stat_out_file << std::endl;
    stat_out_file << "\t\tTotal Space: " << getSize() << std::endl;
    stat_out_file << "\t\tTotal Space Used: " << getUsage() << std::endl;
    stat_out_file << "\t\tPercent Used: " << getUsagePercentage() << "%"  << std::endl;
    stat_out_file << "\t}" << std::endl;

    stat_out_file << std::endl;

    stat_out_file << "\tWarehouse Visualization {" << std::endl;

    for(int i = 0; i < units.size(); i++){
        stat_out_file << "\t\t";
        for(int j = 0; j < units[i].size(); j++){
            stat_out_file << units[i][j].getUsedCapacity() << ":" << units[i][j].getCapacity() << " ";
        }
        stat_out_file << std::endl;
    }

    stat_out_file << "\t}" << std::endl;

    stat_out_file << std::endl;

    stat_out_file << "\tWarehouse Adjacency List {" << std::endl;

    for(int i = 0; i < graph.size(); i++){
        stat_out_file << "\t\t";
        for(int j = 0; j < graph[i].size(); j++){
            stat_out_file << "(" << graph[i][j].src / units.size() << "," << graph[i][j].src % units.size() << ")->(" << graph[i][j].dest / units.size() << "," << graph[i][j].dest % units.size() << "): " << graph[i][j].weight << (j == graph[i].size() - 1 ? "" : ", ");
        }
        stat_out_file<<std::endl;
    }

    stat_out_file << "\t}" << std::endl;

    stat_out_file << "}" << std::endl;

    stat_out_file.close();

    //
    // Exporting StorageUnit Data
    //

    std::ofstream units_out_file("./exports/warehouse_units.csv");

    units_out_file << "Capacity,XCoord,YCoord" << std::endl;
    for(std::vector<StorageUnit> row : units){
        for(StorageUnit u : row){
            if(u.getCapacity() != 0) units_out_file << u.getCapacity() << "," << u.getLocation().first << "," << u.getLocation().second << std::endl;
        }
    }

    units_out_file.close();

    //
    // Exporting Item Data
    //

    std::ofstream items_out_file("./exports/warehouse_items.csv");

    items_out_file << "Name,Quantity,UnitSize" << std::endl;
    for(int i = 0; i < units.size(); i++){
        for(int j = 0; j < units[i].size(); j++){
            auto items = units[i][j].getItems();
            for(auto i : items){
                if(i.second.quantity != 0) items_out_file << i.first << "," << i.second.quantity << "," << i.second.size_per_unit << std::endl;
            }
        }
    }

    items_out_file.close();

    return;
}
