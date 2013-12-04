#ifndef __NEURON_HPP__
#define __NEURON_HPP__

#include <map>
#include <vector>
#include "entity.hpp"
#include "testset.hpp"
#include "constant.hpp"

typedef std::map<Entity*, double> WeightMap;

const double lc_c = 0.5;
const int it_c = 1000000;

val_t signum(val_t);

class Neuron : public Entity {
public:
    Neuron(EntityVector entities);
    Neuron(WeightMap weights);
    Neuron(val_t (*act_func) (val_t), WeightMap weights);
    ~Neuron() {delete bias;}

    val_t val();
    void change_weight(int add);
    double set_learn_const(double lc);
    int set_iterations(int it);
    void print_weights();
    bool learn(TestSet testset, Inputs &inputs);
private:
    bool check(TestSetStruct test, Inputs &inputs);
    int check_all(TestSet testset, Inputs &inputs);
    
    val_t (*act_func) (val_t);
    WeightMap weights;
    double learn_const;
    int iterations;
    Constant *bias;
};

#endif
