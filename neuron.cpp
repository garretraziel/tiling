#include "neuron.hpp"
#include "constant.hpp"
#include <iostream>
#include <cstdlib>

val_t signum(val_t sum) {
    return sum > 0? 1 : -1;
}

Neuron::Neuron(WeightMap weights):
    act_func(signum), weights(weights), learn_const(lc_c), iterations(it_c) {
    bias = new Constant;
    weights[bias] = ((double) rand())/RAND_MAX;
}

Neuron::Neuron(val_t (*act_func) (val_t), WeightMap weights):
    act_func(act_func), weights(weights), learn_const(lc_c), iterations(it_c) {
    bias = new Constant;
    weights[bias] = ((double) rand())/RAND_MAX;
}

Neuron::Neuron(EntityVector entities):
    act_func(signum), learn_const(lc_c), iterations(it_c) {
    EntityVector::iterator it;
    for (it = entities.begin(); it != entities.end(); it++) {
        weights[*it] = ((double) rand())/RAND_MAX;
    }
    bias = new Constant(1);
    weights[bias] = ((double) rand())/RAND_MAX;
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

bool Neuron::check(TestSetStruct test, Inputs &inputs) {
    inputs.set_values(test.inputs);
    return this->val() == test.type;
}

int Neuron::check_all(TestSet testset, Inputs &inputs) {
    int count = 0;
    TestSetVector::iterator it;
    for (it = testset.tests.begin(); it != testset.tests.end(); it++) {
        if (check(*it, inputs)) count++;
    }
    return count;
}

bool Neuron::learn(TestSet testset, Inputs &inputs) {
    int k = 1;
    int best_length = 0;
    int current_length = 0;
    WeightMap pocket = weights;

    while (k < iterations) {
        TestSetStruct sample = testset.tests[rand()%testset.input_length];

        if (check(sample, inputs)) {
            current_length++;
        } else {
            
        }
        
        k++;
    }

    return true;
    
    // bool learned = false;
    // int count = 0;
    // while (!learned && count != iterations) {
    //     learned = true;

    //     TestSetVector::iterator it;
    //     for (it = testset.tests.begin(); it != testset.tests.end(); it++) {
    //         if (!check(it->inputs, it->type, inputs)) {
    //             learned = false;
    //             change_weight(it->type);
    //             print_weights();
    //         }
    //     }
        
    //     count++;
    // }
    // return learned;
}
