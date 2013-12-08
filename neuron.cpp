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
        // nahodne inicializuje vahy pro vsechny vstupy od 0 do 1
        weights[*it] = ((double) rand())/RAND_MAX;
    }
    bias = new Constant(1);
    weights[bias] = ((double) rand())/RAND_MAX;
}

val_t Neuron::val() {
    WeightMap::iterator it;
    val_t sum = 0;
    // neuron se zepta vsech na nej napojenych neuronu na
    // jejich hodnotu, tu pote vynasobi prislusnou vahou
    for (it = weights.begin(); it != weights.end(); it++) {
        sum += it->first->val() * it->second;
    }
    // na vysledek aplikuje aktivacni funkci
    tmp_val = act_func(sum);
    return tmp_val;
}

void Neuron::change_weight(TestSetStruct sample, Inputs &inputs) {
    inputs.set_values(sample.inputs);
    WeightMap::iterator it;
    for (it = weights.begin(); it != weights.end(); it++) {
        // zmeni hodnotu vahy pripojeneho neuronu
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
    unsigned int count = 0; // pocet chyb
    TestSetVector::iterator it;

    unsigned int sum_f = 0; // pocet vzorku tridy 1
    unsigned int sum_s = 0; // pocet vzorku tridy 2
    unsigned int corr_f = 0; // pocet spravne klasifikovanych vzorku prvni tridy
    unsigned int corr_s = 0; // ditto
    val_t first_class = testset[0].type;
    
    for (it = testset.begin(); it != testset.end(); it++) {
        if (it->type == first_class) {
            sum_f++; // navysim pocet vzorku prvni tridy
            if (!check(*it, inputs)) { // pri nespravne klasifikaci
                count++; // navysim pocet chyb
            } else {
                corr_f++; // navysim pocet spravne klasifikovanych vzorku
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

    // pokud neuron neklasifikuje spravne ani jeden vzorek nektere tridy,
    // prestoze se tam takovy vzorek nachazi, vrat "vse spatne"
    // neuron by pak nerozdelil mnozinu vzorku do verohodnych skupin
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
        TestSetStruct sample = testset[rand()%testset.size()]; // vyber nahodny vzorek

        if (check(sample, inputs)) { // pokud spravne klasifikuje, zvys pocet spravnych klasifikaci
            current_length++;
        } else {
            // pokud nespravne klasifikuje, ale pokud jsou aktualni vahy lepsi,
            // nez vahy v kapse, aktualizuj obsah kapsy
            unsigned int actual_errors = check_all(testset, inputs);
            if ((best_length < current_length) && (actual_errors < errors)) {
                best_length = current_length;
                errors = actual_errors;
                pocket = weights;
            }
            // aktualizuj vahy
            change_weight(sample, inputs);
            current_length = 0;
        }
        
        k++;
    }

    // po konci algoritmu musim jeste zkontrolovat, zda jsem ho neukoncil
    // omylem s aktualne nejlepsi hodnotou vah
    if ((best_length < current_length) && (check_all(testset, inputs) < errors)) {
        pocket = weights;
    }

    weights = pocket; // vysledne vahy jsou v promenne pocket

    return check_all(testset, inputs); // vrat pocet chybne klasifikovanych vzorku
}
