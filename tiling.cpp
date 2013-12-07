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
bool smallest_unfaightful(TestSet testset, Inputs &inputs, NeuronVector &current_layer, TestSetVector &result);
int check_network(TestSet testset, Inputs &inputs, Neuron *last_master);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "[ERR] bad arguments, expecting:" << std::endl;
        std::cerr << argv[0] << " train_set data_set" << std::endl;
        return 1;
    }

    srand(time(NULL));
    
    TestSet testset;
    if (!testset.read_testset(argv[1])) {
        std::cerr << "[ERR] cannot read train set file" << std::endl;
        return 1;
    }

    TestSet dataset;
    if (!dataset.read_testset(argv[2])) {
        std::cerr << "[ERR] cannot read test set file" << std::endl;
        return 1;
    }
    
    Inputs inputs(testset.input_length);
    NeuronVector all_neurons;
    NeuronVector current_layer;
    EntityVector previous_layer = inputs.as_entities();
    Neuron *last_master;

    while (true) {
        Neuron *master = new Neuron(previous_layer);
        all_neurons.push_back(master);
        current_layer.push_back(master);
        last_master = master;

        std::cout << "M";
        std::cout.flush();
        if (master->learn(testset.tests, inputs) == 0) {
            break;
        }

        bool all_faightful = false;
        while (!all_faightful) {
            all_faightful = true;
            TestSetVector unfaightful_vector;
            
            if (smallest_unfaightful(testset, inputs, current_layer, unfaightful_vector)) {
                all_faightful = false;
                std::cout << "a";
                std::cout.flush();
                Neuron *ancilliary = new Neuron(previous_layer);
                all_neurons.push_back(ancilliary);
                current_layer.push_back(ancilliary);
                ancilliary->learn(unfaightful_vector, inputs);
            }
        }
        
        previous_layer = copy_neuron_to_entity(current_layer);
        current_layer.clear();
        std::cout << std::endl;
    }

    std::cout << "\nTrain Set:\n";
    check_network(testset, inputs, last_master);
    
    std::cout << "Test set:\n";
    check_network(dataset, inputs, last_master);
        
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

bool smallest_unfaightful(TestSet testset, Inputs &inputs, NeuronVector &current_layer, TestSetVector &result) {
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
    
    std::string smallest_unfaightful_s;
    StrTestMap::iterator stit;
    for (stit = classes.begin(); stit != classes.end(); stit++) {
        if (unfaightful(stit->second)) {
            if (smallest_unfaightful_s == "" || classes[smallest_unfaightful_s].size() > stit->second.size()) {
                smallest_unfaightful_s = stit->first;
            }
        }
    }
    if (smallest_unfaightful_s == "") return false;
    else {
        result = classes[smallest_unfaightful_s];
        return true;
    }
}

int check_network(TestSet testset, Inputs &inputs, Neuron *last_master) {
    int errors = 0;
    TestSetVector::iterator tsit;
    for (tsit = testset.tests.begin(); tsit != testset.tests.end(); tsit++) {
        inputs.set_values(tsit->inputs);
        val_t result = last_master->val();
        if (tsit->type == result) {
            std::cout << " [OK]  " << tsit->type << " == " << result;
        } else {
            std::cout << "[FAIL] " << tsit->type << " != " << result;
            errors++;
        }
        std::cout << ", values:";
        InputVector::iterator iit;
        for (iit = tsit->inputs.begin(); iit != tsit->inputs.end(); iit++) {
            std::cout << " " << *iit;
        }
        std::cout << std::endl;
    }
    return errors;
}
