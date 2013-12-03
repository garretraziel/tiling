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
    PConstantVector inputs;
    WeightMap weights;
    
    for (int i = 0; i < testset.input_length; i++) {
        Constant *c = new Constant();
        inputs.push_back(c);
        weights[c] = 1;
    }

    Neuron n(weights);

    for (int i = 0; i<100; i++) {
        TestSetVector::iterator it;
        for (it = testset.tests.begin(); it != testset.tests.end(); it++) {
            for (unsigned int j = 0; j < it->inputs.size(); j++) {
                inputs[j]->set_val(it->inputs[j]);
            }

            if (n.val() != it->type) {
                n.change_weight(it->type);
                n.print_weights();
            }
            std::cout << "---" << std::endl;
        }
        std::cout << "---------------------" << std::endl;
    }
    n.print_weights();
    
    return 0;
}
