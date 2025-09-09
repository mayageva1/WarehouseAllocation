//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - range_tree.h
//

//hi <3 i think jen is awesome shes so smart -dom

#ifndef RangeTree_H
#define RangeTree_H

#include "../container.h"

//
// CLASS: RTNode
// Represents a node stored in RangeTree. Each node contains a StorageUnit object and pointers to its left and right
// child nodes. The range is calculated using values stored within the StorageUnit instance. The range of each node is
// defined as the free capacity within a StorageUnit instance to the max capacity of a StorageUnit instance.
//

class RTNode{
    public:
        // CONSTRUCTORS
        RTNode();
        RTNode(StorageUnit data);

    private:
        // MEMBER VARIABLES

        // DATA: The node's instance of StorageUnit.
        StorageUnit data;
        // LEFT: A pointer to the node's left child.
        RTNode* left;
        // RIGHT: A pointer to the node's right child.
        RTNode* right;

    // FRIEND CLASS: Declaring RangeTree as a friend class so that private member variables can be accessed from the class.
    friend class RangeTree;
};

//
// REQUIRED DATA STRUCTURE: RangeTree
// Represents a Range Tree data structure. Contains methods to insert new StorageUnit instances, update existing nodes,
// perform range queries, and other operations.
//

class RangeTree {
    private:
        // MEMBER VARIABLES

        // ROOT: The root node of the RangeTree.
        RTNode* root;

        // FUNCTIONS

        // DESTROY: Private recursive helper function to destroy the RangeTree. Called by the class destructor.
        void destroy(RTNode* node);
        // INSERT: Private recursive helper function to insert a node into the RangeTree.
        RTNode* insert(RTNode* node, StorageUnit data);
        // FINDNODE: Private recursive helper function to locate a StorageUnit instance within the RangeTree.
        RTNode* findNode(RTNode* node, std::pair<int, int> loc);
        // RANGEQUERY: Private recursive helper function to perform a range query on the tree.
        void rangeQuery(RTNode *node, std::pair<int, int> size_range, std::vector<StorageUnit>& results);

    public:
        // CONSTRUCTORS
        RangeTree();
        RangeTree(std::vector<StorageUnit> units_in);
        ~RangeTree();

        // FUNCTIONS

        // INSERT: Add a new node to the range tree.
        void insert(StorageUnit data);
        // UPDATENODE: Update an existing node in the range tree. The node is found by its StorageUnit coordinates.
        void updateNode(std::pair<int, int> loc, StorageUnit new_unit);
        // RANGEQUERY: Perform a range query on the tree.
        std::vector<StorageUnit> rangeQuery(std::pair<int, int> size_range);
};

#endif
