//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - range_tree.cpp
//

#include "range_tree.h"

//
// CLASS: RTNode
// Represents a node stored in RangeTree. Each node contains a StorageUnit object and pointers to its left and right
// child nodes. The range is calculated using values stored within the StorageUnit instance. The range of each node is
// defined as the free capacity within a StorageUnit instance to the max capacity of a StorageUnit instance.
//

// CONSTRUCTOR: Default constructor for the RTNode class. The StorageUnit value is not defined and the pointers for the node's
// children are set to null. This constructor is not often used.
RTNode::RTNode(){
    this->left = nullptr;
    this->right = nullptr;
}

// CONSTRUCTOR: Creates an instance of RTNode that contains a StorageUnit instance. The pointers for the node's children
// are still set to null.
RTNode::RTNode(StorageUnit data){
    this->data = data;
    this->left = nullptr;
    this->right = nullptr;
}

//
// REQUIRED DATA STRUCTURE: RangeTree
// Represents a Range Tree data structure. Contains methods to insert new StorageUnit instances, update existing nodes,
// perform range queries, and other operations.
//

// CONSTRUCTOR: Default constructor for the RangeTree class. Sets the root of the tree to be null.
RangeTree::RangeTree(){
    this->root = nullptr;
}

// CONSTRUCTOR: Creates an instance of a RangeTree that contains StorageUnit instances at the time of creation. Constructor
// requires parameter units_in, a vector of StorageUnit instances to be added as nodes into the range tree.
RangeTree::RangeTree(std::vector<StorageUnit> units_in){
    for(StorageUnit i : units_in){
        insert(i);
    }
}

// DESTRUCTOR: Destroys the RangeTree instance. Recursively deletes all RTNodes.
RangeTree::~RangeTree(){
    destroy(this->root);
}

// FUNCTION: Recursive function responsible for deleting a RangeTree instance. The only parameter is node, a RTNode pointer.
// After calling this function, node and all its children are deleted.
void RangeTree::destroy(RTNode *node) {
    if(node != nullptr){
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
    return;
}

// FUNCTION: Public-facing function to insert a StorageUnit instance into the RangeTree. Accepts a StorageUnit instance
// as a parameter.
void RangeTree::insert(StorageUnit value){
    // Checks if the root is null. If so, the new node is assigned to the root value. If not, the recursive helper function
    // is called.
    if(this->root == nullptr) this->root = new RTNode(value);
    else insert(this->root, value);
    return;
}

// FUNCTION: Private helper function to recursively insert a new StorageUnit into the range tree. Accepts parameters node,
// a RTNode pointer, and value, an instance of StorageUnit. Returns a RTNode pointer.
RTNode* RangeTree::insert(RTNode *node, StorageUnit value){
    // If the provided node is null, assign a new node with the provided input to that position.
    if(node == nullptr) return new RTNode(value);

    // Calculating the remaining capacity of the respective StorageUnit instances.
    int capacity = node->data.getCapacity() - node->data.getUsedCapacity();
    int new_unit_capacity = value.getCapacity() - value.getUsedCapacity();

    if(new_unit_capacity < capacity) node->right = insert(node->right, value);
    else node->left = insert(node->left, value);

    return node;
}

// FUNCTION: Locates and updates the StorageUnit in the range tree when new items are added to the StorageUnit. Parameters
// include loc, a pair of integers representing cartesian coordinates, and new_unit, a updated instance of StorageUnit.
void RangeTree::updateNode(std::pair<int, int> loc, StorageUnit new_unit) {
    RTNode* found = findNode(this->root, loc);
    if(found != nullptr) found->data = new_unit;
    return;
}

// FUNCTION: Recursive helper function for updateNode(...). Given a pair of coordinates, this function locates the respective StorageUnit
// instance in the range tree. Returns a RTNode pointer.
RTNode* RangeTree::findNode(RTNode *node, std::pair<int, int> loc) {
    // If the node is null, the StorageUnit does not exist in the range tree.
    if(node == nullptr) return nullptr;
    // Checking if the current node in the traversal is the requested node.
    if(node->data.getLocation() == loc) return node;

    RTNode* l = findNode(node->left, loc);
    if(l != nullptr) return l;
    RTNode* r = findNode(node->right, loc);
    if(r != nullptr) return r;

    return nullptr;
}

// FUNCTION: Public-facing function to perform a range query on the range tree. Accepts parameter size_range, a pair of integers
// representing the range of values to search for within the tree. Returns a vector of StorageUnit instances that match
// the search parameters.
std::vector<StorageUnit> RangeTree::rangeQuery(std::pair<int, int> size_range){
    std::vector<StorageUnit> results;
    rangeQuery(this->root, size_range, results);
    return results;
}

// FUNCTION: Private helper function to recursively perform a range query on the tree. Accepts parameters node, a RTNode
// pointer, size_range, a pair of integers representing the search range, and a reference to results, a vector of StorageUnit
// instances to be returned upon completion of the recursive function.
void RangeTree::rangeQuery(RTNode *node, std::pair<int, int> size_range, std::vector<StorageUnit>& results){
    if(!node) return;

    // Calculating the remaining space in the current node.
    int capacity = node->data.getCapacity() - node->data.getUsedCapacity();
    // Checking the bounds of the search compared to the node.
    if(capacity >= size_range.first && capacity >= size_range.second) {
        // If the node satisfies the range query, push the node's StorageUnit instance to the results vector.
        results.push_back(node->data);
    }else{
        // If not satisfactory, recursively call this function again with the current node's children.
        rangeQuery(node->left, size_range, results);
        rangeQuery(node->right, size_range, results);
    }
    return;
}
