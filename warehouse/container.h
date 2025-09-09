//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - container.h
//

#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

//
// CLASS: Item
// Represents an item stored in a warehouse. Each instance of Item has a name, quantity, and size per unit. The name
// is used to identify the Item and the quantity and size are used to determine how much space each Item instance
// consumes.
//

class Item{
    public:
        // CONSTRUCTORS
        Item();
        Item(std::string name, int quantity, int size);

        // MEMBER VARIABLES

        // NAME: The Item's name.
        std::string name;
        // QUANTITY: The number of items represented by this Item instance.
        int quantity;
        // SIZE_PER_UNIT: How much space each item requires to store.
        int size_per_unit;

        // OPERATOR OVERLOAD

        // OPERATOR +=: Add two Item instances together.
        Item& operator+=(const Item &i);
};

//
// CLASS: Storage Unit
// Represents a storage unit in a warehouse. Each unit has a capacity, location, and a map of Item values.
// The capacity represents the maximum storage capacity of the unit. The location is stored as a pair of cartesian
// coordinates, representing the units relative position in a 2D vector. The Items stored in the unit are managed as a
// map of item names to Item instances.
//

class StorageUnit{
    public:
        // CONSTRUCTORS
        StorageUnit();
        StorageUnit(int capacity, std::pair<int, int> location);
        StorageUnit(int capacity, std::pair<int, int> location, std::vector<Item> items);

        // PUBLIC METHODS

        // FUNCTION: Add an Item to the StorageUnit instance.
        void add(Item i);

        // FUNCTION: Return the max capacity of the StorageUnit instance.
        int getCapacity();
        // FUNCTION: Return the capacity of the StorageUnit instance that is currently in use.
        int getUsedCapacity();
        // FUNCTION: Return the location of the StorageUnit instance.
        std::pair<int, int> getLocation();
        // FUNCTION: Return the StorageUnit instance's map of Items.
        std::map<std::string, Item> getItems();

    private:
        // MEMBER VARIABLES

        // LOCATION: A pair of integers representing the X and Y coordinates of the StorageUnit.
        std::pair<int, int> location;
        // ITEMS: A map of Item instances currently in the StorageUnit.
        std::map<std::string, Item> items;

        // CAPACITY: The max capacity of the StorageUnit instance.
        int capacity;
        // USED_CAPACITY: The capacity of the StorageUnit instance currently in use.
        int used_capacity = 0;
};

#endif
