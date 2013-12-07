#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

#include "constant.hpp"
#include "neuron.hpp"
#include "testset.hpp"

typedef std::vector<int> IntVector;
typedef std::map<std::string, TestSetVector> StrTestMap;
typedef struct {
    double rate;
    int min;
    int max;
} error_rate;

const int folds = 10;

void create_network(TestSetVector testset, Inputs &inputs, NeuronVector &all_neurons, Neuron *&last_master);
EntityVector copy_neuron_to_entity(NeuronVector neurons);
std::string bipolar_to_string(IntVector values);
bool unfaightful(TestSetVector tests);
bool smallest_unfaightful(TestSetVector testset, Inputs &inputs, NeuronVector &current_layer, TestSetVector &result);
int check_network(TestSetVector testset, Inputs &inputs, Neuron *last_master);
error_rate crossvalidate(TestSetVector testset);

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 2) {
        std::cerr << "[ERR] bad arguments, expecting:" << std::endl;
        std::cerr << argv[0] << " train_set data_set" << std::endl;
        std::cerr << "or for ten-fold crossvalidation:" << std::endl;
        std::cerr << argv[0] << " data_set" << std::endl;
        return 1;
    }

    srand(time(NULL));

    if (argc == 3) {
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
        Neuron *last_master;
        create_network(testset.tests, inputs, all_neurons, last_master);
    
        std::cout << "\nTrain Set: " << check_network(testset.tests, inputs, last_master)
                  << " errors out of " << testset.tests.size() << ".\n";
        std::cout << "Test Set: " << check_network(dataset.tests, inputs, last_master)
                  << " errors out of " << dataset.tests.size() << ".\n";
        
        NeuronVector::iterator it;
        for (it = all_neurons.begin(); it != all_neurons.end(); it++) {
            delete *it;
        }
    } else {
        TestSet testset;
        if (!testset.read_testset(argv[1])) {
            std::cerr << "[ERR] cannot read train set file" << std::endl;
            return 1;
        }

        error_rate rate = crossvalidate(testset.tests);
        std::cout << "Ten-times ten-fold crossvalidation showed "
                  << rate.rate << " % error rate.\nMin: " << rate.min
                  << "\nMax: " << rate.max << std::endl;
    }
        
    return 0;
}

void create_network(TestSetVector testset, Inputs &inputs, NeuronVector &all_neurons, Neuron *&last_master) {
    NeuronVector current_layer;
    EntityVector previous_layer = inputs.as_entities();

    while (true) {
        Neuron *master = new Neuron(previous_layer);
        all_neurons.push_back(master);
        current_layer.push_back(master);
        last_master = master;

        std::cout << "M";
        std::cout.flush();
        if (master->learn(testset, inputs) == 0) {
            std::cout << std::endl;
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

bool smallest_unfaightful(TestSetVector testset, Inputs &inputs, NeuronVector &current_layer, TestSetVector &result) {
    StrTestMap classes;
    TestSetVector::iterator it;
    for (it = testset.begin(); it != testset.end(); it++) {
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

int check_network(TestSetVector testset, Inputs &inputs, Neuron *last_master) {
    int errors = 0;
    TestSetVector::iterator tsit;
    for (tsit = testset.begin(); tsit != testset.end(); tsit++) {
        inputs.set_values(tsit->inputs);
        val_t result = last_master->val();
        if (tsit->type != result) {
            errors++;
        }
    }
    return errors;
}

error_rate crossvalidate(TestSetVector testset) {
    error_rate rate; rate.min = -1; rate.max = -1;
    
    int testset_size = testset.size();
    TestSetVector first_class;
    TestSetVector second_class;
    val_t first_type = testset[0].type;

    TestSetVector::iterator it;
    for (it = testset.begin(); it != testset.end(); it++) {
        if (it->type == first_type) {
            first_class.push_back(*it);
        } else {
            second_class.push_back(*it);
        }
    }

    int errors = 0;
    int count = 0;
    for (int i = 0; i < folds; i++) {
        std::random_shuffle(first_class.begin(), first_class.end());
        std::random_shuffle(second_class.begin(), second_class.end());
        
        std::vector<TestSetVector> folds_vector;

        for (int j = 0; j < folds; j++) {
            TestSetVector fold;
            for (int k = 0; k < testset_size/folds/2; k++) {
                fold.push_back(first_class[j*5+k]);
                fold.push_back(second_class[j*5+k]);
            }
            folds_vector.push_back(fold);
        }

        for (int j = 0; j < folds; j++) {
            TestSetVector dataset = folds_vector[j];
            TestSetVector train_set;
            for (int k = 0; k < folds; k++) {
                if (k != j) {
                    train_set.insert(train_set.end(), folds_vector[k].begin(), folds_vector[k].end());
                }
            }

            Inputs inputs(train_set[0].inputs.size());
            NeuronVector all_neurons;
            Neuron *last_master;
            create_network(train_set, inputs, all_neurons, last_master);

            int error = check_network(dataset, inputs, last_master);
            if (error > rate.max) rate.max = error;
            if (error < rate.min || rate.min == -1) rate.min = error;
            
            errors += error;
            count += dataset.size();
        
            NeuronVector::iterator it;
            for (it = all_neurons.begin(); it != all_neurons.end(); it++) {
                delete *it;
            }
            std::cout << "Errors: " << error << "\n-\n";
        }
    }
    rate.rate = ((double) errors)/count*100;
    return rate;
}
