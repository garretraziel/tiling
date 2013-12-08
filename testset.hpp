#ifndef __TESTSET_HPP__
#define __TESTSET_HPP__

#include <vector>
#include <string>
#include "entity.hpp"

typedef std::vector<val_t> InputVector;

// struktura po popis jednoho vzorku
typedef struct {
    InputVector inputs; // mnozina hodnot pro jednotlive vstupy
    val_t type; // trida vzorku
} TestSetStruct;

typedef std::vector<TestSetStruct> TestSetVector;

class TestSet {
public:
    TestSet():input_length(0) {}
    TestSet(std::string filename);

    // nacti vzorky ze souboru
    bool read_testset(std::string filename);
    // delka vstupni vrstvy
    int input_length;
    
    TestSetVector tests; // vektor testu
};

#endif
