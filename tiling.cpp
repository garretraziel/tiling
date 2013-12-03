#include <iostream>
#include "constant.hpp"
#include "neuron.hpp"
#include "testset.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "[ERR] filename argument" << std::endl;
        return 1;
    }
    
    TestSet testset(argv[1]);
    WeightMap weights;
    
    Inputs inputs(testset.input_length);

    PConstantVector::iterator pit;
    for (pit = inputs.input_vector.begin(); pit != inputs.input_vector.end(); pit++) {
        weights[*pit] = 1;
    }

    Constant *bias = new Constant(1);
    weights[bias] = 1;

    Neuron n(weights);

    n.learn(testset, inputs);
    
    n.print_weights();

    delete bias;
    
    return 0;
}
