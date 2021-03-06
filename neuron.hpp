#ifndef __NEURON_HPP__
#define __NEURON_HPP__

#include <map>
#include <vector>
#include "entity.hpp"
#include "testset.hpp"
#include "constant.hpp"

class Neuron;

typedef std::map<Entity*, double> WeightMap;
typedef std::vector<Neuron*> NeuronVector;

const double lc_c = 0.5; // ucici konstanta
const int it_c = 100000; // pocet iteraci pro pocket algoritmus

val_t signum(val_t);

// trida, reprezentujici jeden neuron
class Neuron : public Entity {
public:
    Neuron(EntityVector entities);
    Neuron(WeightMap weights);
    Neuron(val_t (*act_func) (val_t), WeightMap weights);
    ~Neuron() {delete bias;}

    // metoda, vracejici vystup neuronu. neuron
    // se rekurzivne pta vsech na nej napojenych
    // neuronu na jejich val(), dokud nedorazi
    // ke vstupni vrstve
    val_t val();
    double set_learn_const(double lc);
    int set_iterations(int it);
    void print_weights();
    // neuron se nauci pomoci pocket algoritmu klasifikovat
    // data z dane mnoziny
    unsigned int learn(TestSetVector testset, Inputs &inputs);
private:
    // zkontroluj, zda neuron klasifikuje tato data spravne
    bool check(TestSetStruct test, Inputs &inputs);
    // spocitej, kolik vzorku klasifikuje neuron spatne
    unsigned int check_all(TestSetVector testset, Inputs &inputs);
    // zmen vahy neuronu podle hodnot jeho vstupu
    void change_weight(TestSetStruct sample, Inputs &inputs);
    
    val_t (*act_func) (val_t); // aktivacni funkce, signum
    WeightMap weights; // vektor vah
    double learn_const; // ucici konstanta
    int iterations; // pocet iteraci pro pocket
    Constant *bias; // bias
    val_t tmp_val;
};

#endif
