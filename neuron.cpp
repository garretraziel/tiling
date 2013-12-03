#include "neuron.hpp"
#include <iostream>

val_t signum(val_t sum) {
    return sum > 0? 1 : -1;
}

val_t Neuron::val() {
    WeightMap::iterator it;
    val_t sum = 0;
    for (it = weights.begin(); it != weights.end(); it++) {
        sum += it->first->val() * it->second;
    }
    return act_func(sum);
}

void Neuron::change_weight(int add) {
    WeightMap::iterator it;
    for (it = weights.begin(); it != weights.end(); it++) {
        weights[it->first] += it->first->val()*add*2*learn_const;
    }
}

double Neuron::set_learn_const(double lc) {
    double r = learn_const;
    learn_const = lc;
    return r;
}

int Neuron::set_iterations(int it) {
    int r = iterations;
    iterations = it;
    return r;
}

void Neuron::print_weights() {
    WeightMap::iterator it;
    std::cout << "weights:";
    for (it = weights.begin(); it != weights.end(); it++) {
        std::cout << " " << it->second;
    }
    std::cout << std::endl;
}

bool Neuron::learn(TestSet testset, Inputs &inputs) {
    bool learned = false;
    int count = 0;
    while (!learned && count != iterations) {
        learned = true;

        TestSetVector::iterator it;
        for (it = testset.tests.begin(); it != testset.tests.end(); it++) {
            inputs.set_values(it->inputs);

            if (this->val() != it->type) {
                learned = false;
                change_weight(it->type);
                print_weights();
            }
        }
        
        count++;
    }
    return true;
}
