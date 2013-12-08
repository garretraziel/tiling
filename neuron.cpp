#include "neuron.hpp"
#include "constant.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

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
    tmp_val = act_func(sum);
    return tmp_val;
}

void Neuron::change_weight(TestSetStruct sample, Inputs &inputs) {
    inputs.set_values(sample.inputs);
    WeightMap::iterator it;
    for (it = weights.begin(); it != weights.end(); it++) {
        weights[it->first] += it->first->val()*sample.type*2*learn_const;
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
        if (it->first == bias) std::cout << " bias:";
        std::cout << " " << it->second;
    }
    std::cout << std::endl;
}

bool Neuron::check(TestSetStruct test, Inputs &inputs) {
    inputs.set_values(test.inputs);
    return this->val() == test.type;
}

unsigned int Neuron::check_all(TestSetVector testset, Inputs &inputs) {
    unsigned int count = 0;
    TestSetVector::iterator it;

    unsigned int sum_f = 0;
    unsigned int sum_s = 0;
    unsigned int corr_f = 0;
    unsigned int corr_s = 0;
    val_t first_class = testset[0].type;
    
    for (it = testset.begin(); it != testset.end(); it++) {
        if (it->type == first_class) {
            sum_f++;
            if (!check(*it, inputs)) {
                count++;
            } else {
                corr_f++;
            }
        } else {
            sum_s++;
            if (!check(*it, inputs)) {
                count++;
            } else {
                corr_s++;
            }
        }
    }
    
    if ((sum_f != 0 && corr_f == 0) || (sum_s != 0 && corr_s == 0)) return testset.size();
    else return count;
}

unsigned int Neuron::learn(TestSetVector testset, Inputs &inputs) {    
    int k = 1;
    unsigned int best_length = 0;
    unsigned int current_length = 0;
    WeightMap pocket = weights;
    unsigned int errors = check_all(testset, inputs);
    
    while (k < iterations) {
        TestSetStruct sample = testset[rand()%testset.size()];
        //TestSetStruct sample = testset[k%testset.size()];

        if (check(sample, inputs)) {
            current_length++;
        } else {
            unsigned int actual_errors = check_all(testset, inputs);
            if ((best_length < current_length) && (actual_errors < errors)) {
                best_length = current_length;
                errors = actual_errors;
                pocket = weights;
            }
            change_weight(sample, inputs);
            current_length = 0;
        }
        
        k++;
    }

    if ((best_length < current_length) && (check_all(testset, inputs) < errors)) {
        pocket = weights;
    }

    weights = pocket;

    return check_all(testset, inputs);
}
