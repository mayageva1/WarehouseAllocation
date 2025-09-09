//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - warehouse.h
//

#ifndef Warehouse_H
#define Warehouse_H

#include "container.h"
#include "dsa/range_tree.h"
#include "dsa/algorithms.h"

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

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

class Warehouse{
    public:
        // CONSTRUCTORS
        Warehouse();
        Warehouse(std::vector<StorageUnit> units);

        // FUNCTIONS

        // FUNCTION: Add a StorageUnit to the Warehouse without a given location.
        void add_unit(int capacity);
        // FUNCTION: Add a StorageUnit to the Warehouse.
        void add_unit(StorageUnit i);
        // FUNCTION: Add an Item to the Warehouse.
        void add(Item i);

        // FUNCTION: Returns the total capacity of the Warehouse.
        int getSize();
        // FUNCTION: Returns the capacity being used in the Warehouse.
        int getUsage();
        // FUNCTION: Returns the percentage of space used in the Warehouse.
        float getUsagePercentage();

        // FUNCTION: Returns a StorageUnit instance from within the Warehouse.
        StorageUnit getUnit(std::pair<int, int> loc);

        // FUNCTION: Locates all instances of an Item within the Warehouse.
        std::vector<std::pair<int, int> > findItem(std::string i_name);
        // FUNCTION: Calculates the shortest path between an origin point and a series of destinations.
        int getPath(std::pair<int, int> src, std::vector<std::pair<int, int> > dest);
        // FUNCTION: Calculates the shortest path between an origin point and a series of items.
        int getPath(std::pair<int, int> src, std::vector<std::string> items);

        // FUNCTION: Generates and exports various statistics, visualizations, and data.
        void print();

    private:
        // MEMBER VARIABLES

        // NUM_UNITS: The total number of StorageUnit instances present within the Warehouse.
        int num_units = 0;

        // CAPACITY: The total space available between all StorageUnit instances.
        int capacity = 0;
        // USED_CAPACITY: The total space used between all Item instances in the Warehouse.
        int used_capacity = 0;

        // UNITS: A 2D vector that contains all StorageUnit instances.
        std::vector<std::vector<StorageUnit> > units;
        // GRAPH: A 2D vector that contains all GraphEdge instances. This variable is initialized upon calling the buildGraph(...)
        // function housed in the Algorithms class.
        std::vector<std::vector<GraphEdge> > graph;
        // TREE: A RangeTree instance.
        RangeTree tree;
};

#endif
