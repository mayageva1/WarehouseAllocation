//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - algorithms.cpp
//

#include "algorithms.h"

//
// CLASS: Algorithms
// This class' primary function is to provide implementations of the required algorithms. The class hosts the
// implementations of Dijkstra's Algorithm and the solution to the Fractional Knapsack problem. Also included in this
// class is the function to build the adjacency list used by Dijkstra's Algorithm.
//

// CONSTRUCTOR: Default constructor for the Algorithms class. The class has no member variables, and therefore no
// constructor parameters.
Algorithms::Algorithms(){
    
}

// FUNCTION: Helper function for the buildGraph(...) function. Calculates the distance between two storage units. The
// distance is calculated using the Manhattan Distance formula in addition to the size of the StorageUnit classes.
// Parameters are two StorageUnit instances. The function returns an integer representing the distance calculated.
int distance(StorageUnit& u1, StorageUnit& u2) {
    // Coordinates of the two StorageUnits.
    std::pair<int, int> loc1 = u1.getLocation();
    std::pair<int, int> loc2 = u2.getLocation();
    // Distance is calculated through the Manhattan Distance formula in addition to the square root of both StorageUnit
    // capacities. Assuming the theoretical StorageUnit takes the shape of a square, the length of a side of the
    // StorageUnit can be found by taking the square root of the capacity, or area.
    return std::abs(loc1.first - loc2.first) + std::abs(loc1.second - loc2.second) + (int)std::sqrt(u1.getCapacity()) + (int)std::sqrt(u2.getCapacity());
}

// FUNCTION: Builds a graph representing the Warehouse instance's layout. Each StorageUnit represents a node in the
// graph and the edges represent the connections between adjacent StorageUnit instances. The parameter is a 2D vector
// of StorageUnits. The function returns a 2D vector of GraphEdge instances. The GraphEdge structure is defined in
// "algorithms.h".
std::vector <std::vector<GraphEdge>> Algorithms::buildGraph(std::vector <std::vector<StorageUnit>> &units) {
    std::vector<std::vector<GraphEdge> > graph(std::pow(units.size(), 2));

    // Coordinate shifts for neighboring cells.
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    // Nested "for" loop to iterate over each StorageUnit within the Warehouse.
    for(int i = 0; i < units.size(); i++){
        for(int j = 0; j < units[i].size(); j++){
            // Calculating the index of the StorageUnit in the adjacency list vector.
            int src = i * units[i].size() + j;

            // Iterate over neighboring StorageUnits.
            for(int k = 0; k < 4; k++){
                int x = i + dx[k];
                int y = j + dy[k];
                // Check that the neighboring StorageUnit exists.
                if (x >= 0 && x < units.size() && y >= 0 && y < units[i].size()){
                    // Calculating the index of the StorageUnit in the adjacency list vector.
                    int dest = x * units[i].size() + y;
                    // Call to the distance(...) helper function to calculate the weight of the GraphEdge.
                    int weight = distance(units[i][j], units[x][y]);
                    // Creates a GraphEdge representing the connection between the two StorageUnits.
                    graph[src].push_back({src, dest, weight});
                }
            }
        }
    }
    // Returns the completed graph.
    return graph;
}

// OPERATOR OVERLOAD: Allows the usage of the ">" operator on GraphEdge structures. Compares the two instances based on
// their weights.
bool operator>(const GraphEdge& a, const GraphEdge& b) {
    return a.weight > b.weight;
}

// FUNCTION: Helper function for the dijkstra(...) function. Converts the coordinates of a StorageUnit in the Warehouse
// instance to the index of a StorageUnit in the graph / adjacency list. Accepts two parameters, loc, a pair of integers
// representing the cartesian coordinates, and units_size, an integer representing the width of the Warehouse instance.
// Returns an integer representing the calculated index.
int coordToIndex(std::pair<int, int> loc, int units_size){
    return loc.first * units_size + loc.second;
}

// FUNCTION: Helper function for the dijkstra(...) function. Converts the index of a StorageUnit in the graph / adjacency
// list to a pair of integers representing the coordinates of a StorageUnit in the Warehouse instance.
std::pair<int, int> indexToCoordinates(int idx, int units_size) {
    int x = idx / units_size;
    int y = idx % units_size;
    return {x, y};
}

// REQUIRED ALGORITHM: Calculates the shortest path between two graph nodes using Dijkstra's Algorithm. Accepts parameters
// units, a 2D vector of StorageUnit instances representing the warehouse, graph, a 2D vector of GraphEdge instances
// representing the connections between StorageUnits in the Warehouse instance, src_c, a pair of integers representing
// the starting coordinates to search from, and dest_c, another pair of integers representing the destination's
// coordinates. The function returns a pair consisting of an integer and a vector of integer pairs; the lone integer
// represents the shortest distance between the two given graph nodes and the vector of integer pairs represents the
// respective coordinates along the shortest path.
std::pair<int, std::vector<std::pair<int, int> > > Algorithms::dijkstra(std::vector<std::vector<StorageUnit> >& units, std::vector<std::vector<GraphEdge>>& graph, std::pair<int, int> src_c, std::pair<int, int> dest_c) {
    // Converting the Warehouse coordinates to their respective graph index.
    int src = coordToIndex(src_c, units.size());
    int dest = coordToIndex(dest_c, units.size());
    // Vector to store the distance from the source node to each node in the graph.
    std::vector<int> distance(graph.size(), INT_MAX);
    // Vector to store the previous nodes in the shortest path.
    std::vector<int> previous(graph.size(), -1);
    // Vector to mark wheter each node has been visited yet during traversal.
    std::vector<bool> visited(graph.size(), false);

    // Priority queue to store the graph edges, prioritized based on their weights. The edge with the smallest weight
    // will be at the top of the queue.
    std::priority_queue<GraphEdge, std::vector<GraphEdge>, std::greater<GraphEdge> > p_queue;

    // Marking the distance from the source node to itself as 0 and pushing the source node to the queue to begin traversal.
    distance[src] = 0;
    p_queue.push(GraphEdge({src, src, 0}));

    // Primary loop for Dijkstra's Algorithm.
    while(!p_queue.empty()){
        GraphEdge current = p_queue.top();
        p_queue.pop();

        // Checking if the current node has already been visited and marking it as visited if not.
        if(visited[current.dest]) continue;
        visited[current.dest] = true;

        // Traversing the nodes connected to the current node.
        for(GraphEdge& e : graph[current.dest]){
            // If the neighboring node has not been visited and the distance to that node through the current edge is
            // less than the previously recorded distance, the distance to that node is updated with the shorter distance.
            if(!visited[e.dest] && distance[current.dest] + e.weight < distance[e.dest]) {
                // Update the distance to the next node with the new shorter distance.
                distance[e.dest] = distance[current.dest] + e.weight;
                // Update the vector of previous nodes.
                previous[e.dest] = current.dest;
                // The GraphEdge is added to the queue.
                p_queue.push(GraphEdge({current.dest, e.dest, distance[e.dest]}));
            }
        }
    }

    // Reconstructing the shortest path between the two given points.
    std::vector<std::pair<int, int> > shortest_path;
    int index = dest;

    // Traverse the vector containing previous nodes backward.
    while(index != src){
        // Convert the index back to Warehouse coordinates.
        std::pair<int, int> loc = indexToCoordinates(index, units.size());

        shortest_path.push_back({loc.first, loc.second});
        index = previous[index];
    }

    // Adding the coordinates for the source StorageUnit to the vector.
    shortest_path.push_back({src_c.first, src_c.second});
    // Reverse the order of coordinates to return the correct order.
    std::reverse(shortest_path.begin(), shortest_path.end());

    // Returning the int for the distance of the shortest path and the vector of nodes traversed.
    return {distance[dest], shortest_path};
}

// FUNCTION:: Helper function for the fknapsack(...) function. Compares two ItemRatio instances to determine if instance
// "a" is smaller than instance "b". Returns a boolean; returns true if "a" is smaller than "b" and false if not.
bool compare(ItemRatio a, ItemRatio b) {
    return a.ratio < b.ratio;
}

// REQUIRED ALGORITHM: Implements the Fractional Knapsack algorithm to efficiently distribute items among StorageUnit
// instances within a Warehouse instance. Accepts parameters units, a 2D vector of StorageUnit instances representing the
// warehouse, and i, an instance of Item to be inserted into the warehouse. This algorithm is used when an entire Item
// instance cannot fit into a single StorageUnit instance. The function returns a pair of an integer and a vector of
// integer pairs. The integer represents the space used by the split Item instance able to fit in the Warehouse instance.
// The vector of integers represents the StorageUnit instances modified by the function; this will be used to update the
// RangeTree data structure in the Warehouse instance. The ItemRatio structure is defined in "algorithms.h".
std::pair<int, std::vector<std::pair<int, int> > > Algorithms::fknapsack(std::vector<std::vector<StorageUnit> >& units, Item i) {
    // Vector to store item-capacity ratios.
    std::vector<ItemRatio> ratios;
    // Vector to store the coordinates of units that have had additional items added.
    std::vector<std::pair<int, int>> updated_units;

    // Calculates the item-capacity ratios for all StorageUnit instances in the Warehouse instance.
    for(int x = 0; x < units.size(); x++){
        for(int y = 0; y < units[x].size(); y++){
            double ratio = (double)i.quantity/i.size_per_unit;
            ratios.push_back(ItemRatio(i, ratio,{x, y}));
        }
    }

    // Sorts the vector of ItemRatio instances in descending order.
    std::sort(ratios.begin(), ratios.end(), compare);

    // Integer used to track the total space used by the distributed Item instance.
    int used_space = 0;

    // Iterate through the sorted item-capacity ratios to distribute the Item instance to storage units.
    for(ItemRatio r : ratios){
        StorageUnit& u = units[r.loc.first][r.loc.second];
        // Adds a portion of the Item instance to the current StorageUnit instance if there is available space and there
        // is still a portion of the Item instance left to distribute.
        if(i.quantity > 0 && u.getCapacity() - u.getUsedCapacity() > 0) {
            // Calculates the quantity of the Item instance to store in the current unit.
            int q = std::min((u.getCapacity() - u.getUsedCapacity()) / i.size_per_unit, i.quantity);
            // Add the calculated quantity of the Item to the StorageUnit, push the StorageUnit to the modified vector,
            // update the remaining quantity, and update the total space used.
            u.add(Item(i.name, q, i.size_per_unit));
            updated_units.push_back(u.getLocation());
            i.quantity -= q;
            used_space += q * i.size_per_unit;
        }
    }
    // Returns the total space used by the distributed items and a vector of coordinates representing the modified StorageUnit
    // instances.
    return {used_space, updated_units};
}
