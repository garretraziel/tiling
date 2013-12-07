#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include "constant.hpp"
#include "neuron.hpp"
#include "testset.hpp"

typedef std::vector<int> IntVector;
typedef std::map<std::string, TestSetVector> StrTestMap;

EntityVector copy_neuron_to_entity(NeuronVector neurons);
std::string bipolar_to_string(IntVector values);
bool unfaightful(TestSetVector tests);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "[ERR] filename argument" << std::endl;
        return 1;
    }

    srand(time(NULL));
    
    TestSet testset;
    if (!testset.read_testset(argv[1])) {
        std::cerr << "[ERR] cannot read testset file" << std::endl;
        return 1;
    }
    
    Inputs inputs(testset.input_length);
    NeuronVector all_neurons;
    NeuronVector current_layer;
    EntityVector previous_layer = inputs.as_entities();
    Neuron *last_master;

    while (true) {
        std::cout << "adding new layer\n";
        Neuron *master = new Neuron(previous_layer);
        all_neurons.push_back(master);
        current_layer.push_back(master);
        last_master = master;

        std::cout << "adding master unit" << std::endl;
        if (master->learn(testset.tests, inputs) == 0) {
            break;
        }
        // std::cout << "learned master." << std::endl;

        bool all_faightful = false;
        while (!all_faightful) {
            all_faightful = true;
            StrTestMap classes;
            TestSetVector::iterator it;
            for (it = testset.tests.begin(); it != testset.tests.end(); it++) {
                inputs.set_values(it->inputs);
            
                IntVector prototype;
                NeuronVector::iterator nit;
                for (nit = current_layer.begin(); nit != current_layer.end(); nit++) {
                    prototype.push_back((*nit)->val());
                }

                std::string class_rep = bipolar_to_string(prototype);
                if (classes.find(class_rep) == classes.end()) {
                    TestSetVector vector;
                    classes[class_rep] = vector;
                }

                classes[class_rep].push_back(*it);
            }

            std::string smallest_unfaightful;
            StrTestMap::iterator stit;
            for (stit = classes.begin(); stit != classes.end(); stit++) {
                // std::cout << stit->first << " " << stit->second.size() << std::endl;
                if (unfaightful(stit->second)) {
                    if (smallest_unfaightful == "" || classes[smallest_unfaightful].size() > stit->second.size()) {
                        smallest_unfaightful = stit->first;
                    }
                }
            }
            if (smallest_unfaightful != "") {
                // std::cout << "smallest unfaightful: " << smallest_unfaightful << std::endl;
                all_faightful = false;
                std::cout << "adding ancillary unit" << std::endl;
                Neuron *ancilliary = new Neuron(previous_layer);
                all_neurons.push_back(ancilliary);
                current_layer.push_back(ancilliary);
                // std::cout << "test size: " << classes[smallest_unfaightful].size() << std::endl;
                ancilliary->learn(classes[smallest_unfaightful], inputs);
                // std::cout << "learned\n";
            }
        }
        
        previous_layer = copy_neuron_to_entity(current_layer);
        current_layer.clear();
    }

    TestSetVector::iterator tsit;
    for (tsit = testset.tests.begin(); tsit != testset.tests.end(); tsit++) {
        inputs.set_values(tsit->inputs);
        if (tsit->type == last_master->val()) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL!" << std::endl;
        }
    }
    
    NeuronVector::iterator it;
    for (it = all_neurons.begin(); it != all_neurons.end(); it++) {
        delete *it;
    }
    
    return 0;
}

EntityVector copy_neuron_to_entity(NeuronVector neurons) {
    EntityVector entities;
    NeuronVector::iterator it;
    for (it = neurons.begin(); it != neurons.end(); it++) {
        entities.push_back(*it);
    }
    return entities;
}

std::string bipolar_to_string(IntVector values) {
    std::string result;
    IntVector::iterator it;
    for (it = values.begin(); it != values.end(); it++) {
        result += (*it) == -1? "0" : "1";
    }
    return result;
}

bool unfaightful(TestSetVector tests) {
    if (tests.size() == 0) return false;
    val_t first_type = tests[0].type;
    TestSetVector::iterator it;
    for (it = tests.begin(); it != tests.end(); it++) {
        if (it->type != first_type) return true;
    }
    return false;
}
