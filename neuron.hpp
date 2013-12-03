#ifndef __NEURON_HPP__
#define __NEURON_HPP__

#include <map>
#include <vector>
#include "entity.hpp"
#include "testset.hpp"
#include "constant.hpp"

typedef std::map<Entity*, double> WeightMap;

val_t signum(val_t);

class Neuron : public Entity {
public:
    Neuron():act_func(signum), learn_const(0.5), iterations(10000) {}
    Neuron(WeightMap weights):
        act_func(signum), weights(weights), learn_const(0.5), iterations(10000) {}
    Neuron(val_t (*act_func) (val_t), WeightMap weights):
        act_func(act_func), weights(weights), learn_const(0.5), iterations(10000) {}

    val_t val();
    void change_weight(int add);
    double set_learn_const(double lc);
    int set_iterations(int it);
    void print_weights();
    bool learn(TestSet testset, Inputs &inputs);
private:
    val_t (*act_func) (val_t);
    WeightMap weights;
    double learn_const;
    int iterations;
};

#endif
