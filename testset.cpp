#include "testset.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

TestSet::TestSet(std::string filename) {
    read_testset(filename);
}

bool TestSet::read_testset(std::string filename) {
    std::ifstream fin(filename.c_str());

    if (fin.is_open()) {
        std::string line;
        while (std::getline(fin, line)) {
            std::istringstream ss(line);
            val_t type;
            ss >> type;
            InputVector values;
            while (ss.good()) {
                val_t value;
                ss >> value;
                values.push_back(value);
            }
            input_length = values.size();
            TestSetStruct testset = {values, type};
            tests.push_back(testset);
        }        
    } else {
        std::cerr << "[ERR] File " << filename << " cannot be opened." << std::endl;
        return false;
    }
    
    fin.close();
    return true;
}
