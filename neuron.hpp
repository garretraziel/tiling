#ifndef __NEURON_HPP__
#define __NEURON_HPP__

#include <map>
#include <vector>
#include "entity.hpp"

typedef std::map<Entity*, double> WeightMap;

val_t signum(val_t);

class Neuron : public Entity {
public:
    Neuron():act_func(signum) {}
    Neuron(WeightMap weights):act_func(signum), weights(weights) {}
    Neuron(val_t (*act_func) (val_t), WeightMap weights):
        act_func(act_func), weights(weights) {}

    val_t val();
    
private:
    val_t (*act_func) (val_t);
    WeightMap weights;
};

#endif
