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

const double lc_c = 0.5;
const int it_c = 10000;

val_t signum(val_t);

class Neuron : public Entity {
public:
    Neuron(EntityVector entities);
    Neuron(WeightMap weights);
    Neuron(val_t (*act_func) (val_t), WeightMap weights);
    ~Neuron() {delete bias;}

    val_t val();
    double set_learn_const(double lc);
    int set_iterations(int it);
    void print_weights();
    unsigned int learn(TestSetVector testset, Inputs &inputs);
private:
    bool check(TestSetStruct test, Inputs &inputs);
    unsigned int check_all(TestSetVector testset, Inputs &inputs);
    void change_weight(TestSetStruct sample, Inputs &inputs);
    
    val_t (*act_func) (val_t);
    WeightMap weights;
    double learn_const;
    int iterations;
    Constant *bias;
};

#endif
