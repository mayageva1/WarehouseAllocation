//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - container.cpp
//

#include "container.h"

//
// CLASS: Storage Unit
// Represents a storage unit in a warehouse. Each unit has a capacity, location, and a map of Item values.
// The capacity represents the maximum storage capacity of the unit. The location is stored as a pair of cartesian
// coordinates, representing the units relative position in a 2D vector. The Items stored in the unit are managed as a
// map of item names to Item instances.
//

// CONSTRUCTOR: Default constructor for the StorageUnit class. Capacity is set to 0. When the default constructor is
// used, coordinates are assigned at a later time.
StorageUnit::StorageUnit(){
    this->capacity = 0;
}

// CONSTRUCTOR: Creates an instance of a StorageUnit that has a capacity and location. Parameters are capacity, an integer
// representing the maximum size of the unit, and location, a pair of integers to represent the coordinate location of
// the StorageUnit. There are no items in the StorageUnit at the time of creation.
StorageUnit::StorageUnit(int capacity, std::pair<int, int> location){
    this->capacity = capacity;
    this->location = location;
}

// CONSTRUCTOR: Creates an instance of a StorageUnit that contains items at the time of creation. Constructor has an
// additional parameter, items, a vector of Item instances, to be added to the StorageUnit.
StorageUnit::StorageUnit(int capacity, std::pair<int, int> location, std::vector<Item> items){
    this->capacity = capacity;
    this->location = location;

    for(Item i : items){
        this->add(i);
    }
}

// FUNCTION: Adds an item to the StorageUnit instance. The only parameter is an Item instance.
void StorageUnit::add(Item i) {
    // Check if the item exceeds the max capacity of the StorageUnit instance.
    if(i.quantity * i.size_per_unit + used_capacity > capacity) {
        std::cout << "[StorageUnit Add Error] Unable to store " <<  (i.size_per_unit * i.quantity) << " " << i.name << "(s) due to lack of available storage space.";
        return;
    }
    // If the item exists already in the StorageUnit and the size_per_unit matches, add the new item to the existing
    // item, else create a new entry in the StorageUnit's map.
    if(items[i.name].size_per_unit == i.size_per_unit) items[i.name] += i;
    else items[i.name] = i;

    // Updates the StorageUnit instance variable used_capacity to reflect the additional space that has now been used
    // by adding this new item.
    this->used_capacity += i.quantity * i.size_per_unit;
    return;
}

// FUNCTION: Returns the max capacity of the StorageUnit instance as an integer.
int StorageUnit::getCapacity() {
    return this->capacity;
}

// FUNCTION: Returns the currently used capacity of the StorageUnit instance as an integer.
int StorageUnit::getUsedCapacity() {
    return this->used_capacity;
}

// FUNCTION: Returns the location of the StorageUnit relevant to the Warehouse class' 2D vector as a pair of integers
// representing cartesian coordinates.
std::pair<int, int> StorageUnit::getLocation() {
    return this->location;
}

// FUNCTION: Returns the map of Items stored in the StorageUnit instance.
std::map<std::string, Item> StorageUnit::getItems() {
    return items;
}

//
// CLASS: Item
// Represents an item stored in a warehouse. Each instance of Item has a name, quantity, and size per unit. The name
// is used to identify the Item and the quantity and size are used to determine how much space each Item instance
// consumes.
//

// CONSTRUCTOR: Default constructor for the Item class.
Item::Item(){

}

// CONSTRUCTOR: Primary constructor for the Item class. Accepts parameters name, a string, quantity, and size,
// both integers. Assigns each parameter to its respective class variable.
Item::Item(std::string name, int quantity, int size){
    this->name = name;
    this->quantity = quantity;
    this->size_per_unit = size;
}

// OPERATOR OVERLOAD: Allows the usage of the "+=" operator on two items. Calling this operation will add their
// quantities together.
Item& Item::operator+=(const Item& i){ //item += another item, will add the quantity to an already exisitng item
    this->quantity += i.quantity;
    return (*this);
}
