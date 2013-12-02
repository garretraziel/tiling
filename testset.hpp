#ifndef __TESTSET_HPP__
#define __TESTSET_HPP__

#include <vector>
#include <string>
#include "entity.hpp"

typedef std::vector<val_t> InputVector;

typedef struct {
    InputVector inputs;
    val_t type;
} TestSetStruct;

typedef std::vector<TestSetStruct> TestSetVector;

class TestSet {
public:
    TestSet() {}
    TestSet(std::string filename);

    bool read_testset(std::string filename);
    
    TestSetVector tests;
};

#endif
