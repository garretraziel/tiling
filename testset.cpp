#include "testset.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

TestSet::TestSet(std::string filename) {
    read_testset(filename);
}

bool TestSet::read_testset(std::string filename) {
    std::ifstream fin(filename.c_str());

    if (fin.is_open()) {
        // pro kazdy radek
        std::string line;
        while (std::getline(fin, line)) {
            std::istringstream ss(line);
            // prvni hodnota je trida (1/-1)
            val_t type;
            ss >> type;
            // pote nasleduji hodnoty vstupni vrstvy
            InputVector values;
            while (ss.good()) {
                val_t value;
                ss >> value;
                values.push_back(value);
            }
            input_length = values.size();
            TestSetStruct testset = {values, type};
            tests.push_back(testset); // ulozim vzorek
        }        
    } else {
        return false;
    }
    
    fin.close();
    return true;
}
