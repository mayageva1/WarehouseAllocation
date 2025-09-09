//
// CSC 212 - Data Structures and Abstractions
// Dominic Tucchio, Jennipher Day, Maya Geva, Thomas Williams
// Spring 2024 Term Project - main.cpp
//

#include "./warehouse/warehouse.h"
#include <sstream>

// MAIN FUNCTION: Main function of the program. Fun fact: this project contains 1,351 lines of code!
int main(int argc, char*argv[]){
    // Check for the correct number of command line arguments.
    if (argc < 3) {
        std::cout << "[Warehouse Build Error] Incorrect number of command line arguments.\nUsage: ./warehouse <unitdata.csv> <itemdata.csv> [commands.txt]" << std::endl;
        return 0;
    }

    // Parsing filenames from the command line arguments.
    std::string units_csv_file_name(argv[1]);
    std::string items_csv_file_name(argv[2]);

    //
    // Importing StorageUnit Data
    //

    // Open the file and initialize vectors to store information.
    std::ifstream units_in_file(units_csv_file_name);
    std::vector<StorageUnit> units;
    std::vector<int> unassigned_units;

    // Check to ensure the provided file is valid.
    if (!units_in_file) {
        std::cout << "[Warehouse Build Error] Unable to open units input file." << std::endl;
        return 1;
    }


    std::string line, entry;
    std::getline(units_in_file, line);
    // Parsing each line of the CSV file for StorageUnit instances.
    while(std::getline(units_in_file, line)){
        std::stringstream units_stream(line);
        // Setting the default value to -1 to easily check if they've been overwritten by data from the CSV.
        int capacity, x, y = -1;

        // Parse each value in a line by commas.
        for(int i = 0; i < 3; i++){
            std::getline(units_stream, entry, ',');
            int u = std::stoi(entry);
            if(i == 0) capacity = u;
            if(i == 1) x = u;
            if(i == 2) y = u;
        }

        if(capacity < 0) {
            std::cout << "[Warehouse Build Error] Invalid StorageUnit constructor value found in the provided CSV file.\nUsage: <Capacity>,[XCoord],[YCoord]" << std::endl;
            continue;
        }

        // Check for valid coordinates and push the StorageUnit to the back of the vector. If there are no coordinates or
        // the coordinates are invalid, the capacity is pushed to a separate vector to be assigned to the first available
        // space in the Warehouse.
        if(x >= 0 && y >= 0) units.push_back(StorageUnit(capacity, {x, y}));
        else unassigned_units.push_back(capacity);
    }

    // Closing the StorageUnit CSV file.
    units_in_file.close();

    //
    // INITIALIZING THE WAREHOUSE CLASS
    //

    // The vector of complete StorageUnits is passed in to the constructor of Warehouse. Any additional partial StorageUnits
    // are added after the defined StorageUnits have been added.
    Warehouse w(units);
    for(int i : unassigned_units) w.add_unit(i);

    //
    // Importing Item Data
    //

    // Open the file and initialize vectors to store information.
    std::ifstream items_in_file(items_csv_file_name);
    std::vector<Item> items;

    // Check to ensure the provided file is valid.
    if (!items_in_file) {
        std::cout << "[Warehouse Build Error] Unable to open items input file." << std::endl;
        return 1;
    }


    std::getline(items_in_file, line);
    // Parsing each line of the CSV file for Item instances.
    while(std::getline(items_in_file, line)){
        std::stringstream units_stream(line);
        std::string name;
        // Setting the default value to -1 to easily check if they've been overwritten by data from the CSV.
        int quantity, unit_size = -1;

        // Parse each value in a line by commas.
        for(int i = 0; i < 3; i++){
            std::getline(units_stream, entry, ',');
            if(i == 0) name = entry;
            if(i == 1) quantity = std::stoi(entry);
            if(i == 2) unit_size = std::stoi(entry);
        }

        if(name.empty() || quantity < 0 || unit_size < 0) {
            std::cout << "[Warehouse Build Error] Invalid Item constructor value found in the provided CSV file.\nUsage: <Name>,<Quantity>,<SizePerUnit>" << std::endl;
            continue;
        }

        // Push the Item to a vector of Items that will be added to the Warehouse later.
        items.push_back(Item(name, quantity, unit_size));
    }

    // Closing the Items CSV file.
    items_in_file.close();

    // Inserting Items from the CSV file.
    for(Item i : items) w.add(i);

    //
    // IMPORTING AND HANDLING COMMANDS
    //

    // Commands are not required for the Warehouse program to run. This code only runs if a command file is provided via
    // command line argument.
    if(argc == 4){
        // Open the file.
        std::string commands_txt_file_name(argv[3]);
        std::ifstream commands_in_file(commands_txt_file_name);

        // Check to ensure the provided file is valid.
        if (!commands_in_file) {
            std::cout << "[Warehouse Build Error] Unable to open command input file." << std::endl;
            return 1;
        }

        // Parse through each command.
        while(std::getline(commands_in_file, line)){
            std::stringstream command_stream(line);
            std::string command, parameter;
            // Parsing the command.
            command_stream >> command;

            // Parsing command arguments.
            std::vector<std::string> parameters;
            while(command_stream >> parameter) parameters.push_back(parameter);

            // Command switcher. Each command checks the parameters to ensure they are valid and calls the appropriate
            // function. If there is a syntax error the user is notified via command line and given the correct syntax.
            if(command == "ADD_UNIT"){
                if((parameters.size() != 1 && parameters.size() != 3) || std::stoi(parameters[0]) < 0){
                    std::cout << "[Command Error] Invalid StorageUnit constructor value found in the provided TXT file.\nUsage: ADD_UNIT <Name> <Quantity> <SizePerUnit>" << std::endl;
                    continue;
                }
                if(parameters.size() == 1) w.add_unit(std::stoi(parameters[0]));
                else w.add_unit(StorageUnit(std::stoi(parameters[0]), {std::stoi(parameters[1]), std::stoi(parameters[2])}));
            }
            else if(command == "ADD_ITEM"){
                if(parameters.size() != 3 || parameters[0].empty() || std::stoi(parameters[1]) < 0 || std::stoi(parameters[2]) < 0) {
                    std::cout << "[Command Error] Invalid Item constructor value found in provided TXT file.\nUsage: ADD_ITEM <Name> <Quantity> <SizePerUnit>\n" << std::endl;
                    continue;
                }
                w.add(Item(parameters[0], std::stoi(parameters[1]), std::stoi(parameters[2])));
            }
            else if(command == "FIND_ITEM"){
                if(parameters.size() != 1 || parameters[0].empty()){
                    std::cout << "[Command Error] Invalid invocation of FIND_ITEM found in the provided TXT file.\nUsage: FIND_ITEM <Name>\n" << std::endl;
                    continue;
                }
                std::vector<std::pair<int, int> > results = w.findItem(parameter);
                if(results.empty()) std::cout << "[FIND_ITEM] The provided item \"" << parameters[0] << "\" was not found in the Warehouse.\n";
                else {
                    std::cout << "[FIND_ITEM] " << parameters[0] << " was found in the StorageUnit(s) located at ";
                    for(std::pair<int, int> loc : results){
                        std::cout << "(" << loc.first << "," << loc.second << ") ";
                    }
                    std::cout << "\n" << std::endl;
                }
            }
            else if(command == "FIND_PATH_UNITS"){
                if(parameters.size() < 4 || (std::stoi(parameters[0]) < 0) || (std::stoi(parameters[1]) < 0) || (std::stoi(parameters[2]) < 0) || (std::stoi(parameters[3]) < 0)  ||  (parameters.size() % 2 != 0)){
                    std::cout << "[Command Error] Invalid invocation of FIND_PATH_ITEMS found in the provided TXT file.\nUsage: FIND_PATH_ITEMS <ORIGIN_XCoord> <ORIGIN_YCoord> <DEST_XCoord> <DEST_YCoord> [DEST_XCoord] [DEST_YCoord]...\n" << std::endl;
                    continue;
                }
                std::pair<int, int> origin = {std::stoi(parameters[0]), std::stoi(parameters[1])};
                std::vector<std::pair<int, int> > dest_coords;
                for(int i = 2; i < parameters.size(); i += 2) dest_coords.push_back({std::stoi(parameters[i]), std::stoi(parameters[i+1])});
                w.getPath(origin, dest_coords);
                std::cout << std::endl;
            }
            else if(command == "FIND_PATH_ITEMS"){
                if(parameters.size() < 3 || std::stoi(parameters[0]) < 0 || std::stoi(parameters[1]) < 0 || parameters[2].empty()){
                    std::cout << "[Command Error] Invalid invocation of FIND_PATH_ITEMS found in the provided TXT file.\nUsage: FIND_PATH_ITEMS <ORIGIN_XCoord> <ORIGIN_YCoord> <Item Name> [Item Name]...\n" << std::endl;
                    continue;
                }
                std::pair<int, int> origin = {std::stoi(parameters[0]), std::stoi(parameters[1])};
                std::vector<std::string> items;
                for(int i = 2; i < parameters.size(); i++) items.push_back(parameters[i]);
                w.getPath(origin, items);
                std::cout << std::endl;
            } else std::cout << "[Command Error] Invalid command found in the provided TXT file.\n" << std::endl;
        }
    }

    // Exporting all data relevant to the Warehouse instance. Exports a TXT file containing Warehouse statistics and visualizations,
    // a CSV file for the updated StorageUnit instances, and a CSV file for the updated Item instances.
    w.print();
    return 1;
}
