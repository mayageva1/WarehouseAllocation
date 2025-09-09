//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - algorithms.h
//

#ifndef Algorithms_H
#define Algorithms_H

#include "../container.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

//
// STRUCTURE: GraphEdge
// Represents an edge in a graph. Contains the index of the source node, the index of the destination node, and the
// distance (weight) between nodes. These values are all stored as integers. The index for the source node and destination
// node are converted from the standard (x,y) coordinates used in the main Warehouse instance.
//

struct GraphEdge {
    int src, dest, weight;
};

//
// STRUCTURE: ItemRatio
// A structure combining an Item with its corresponding ratio and location in the Warehouse instance. These variables are
// stored in the form of an Item instance, a double, and a pair of integers respectively.
//

struct ItemRatio {
    Item data;
    double ratio;
    std::pair<int, int> loc;

    // CONSTRUCTOR: Initializes the ItemRatio object with the provided values.
    ItemRatio(Item i, double ratio, std::pair<int, int> loc){
        this->data = i;
        this->loc = loc;
        this->ratio = ratio;
    }
};

//
// CLASS: Algorithms
// This class' primary function is to provide implementations of the required algorithms. The class hosts the
// implementations of Dijkstra's Algorithm and the solution to the Fractional Knapsack problem. Also included in this
// class is the function to build the adjacency list used by Dijkstra's Algorithm.
//

class Algorithms{
    public:
        // CONSTRUCTORS
        Algorithms();

        // PUBLIC METHODS

        // FUNCTION: Construct a graph based on Warehouse instance. Resulting graph is to be used with Dijkstra's Algorithm.
        std::vector<std::vector<GraphEdge> > buildGraph(std::vector<std::vector<StorageUnit> >& units);
        // FUNCTION: Find the shortest distance between two nodes in a graph.
        std::pair<int, std::vector<std::pair<int, int> > > dijkstra(std::vector<std::vector<StorageUnit> >& units, std::vector<std::vector<GraphEdge>>& graph, std::pair<int, int> src_c, std::pair<int, int> dest_c);
        // FUNCTION: Distributes an Item instance among StorageUnit instances.
        std::pair<int, std::vector<std::pair<int, int> > > fknapsack(std::vector<std::vector<StorageUnit> >& units, Item i);
};

#endif
