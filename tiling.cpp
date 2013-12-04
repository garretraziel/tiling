#include <iostream>
#include <cstdlib>
#include <ctime>
#include "constant.hpp"
#include "neuron.hpp"
#include "testset.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "[ERR] filename argument" << std::endl;
        return 1;
    }

    srand(time(NULL));
    
    TestSet testset(argv[1]);
    Inputs inputs(testset.input_length);
    EntityVector entities(inputs.as_entities());

    Neuron n(entities);
    n.learn(testset, inputs);
    n.print_weights();
    
    return 0;
}
