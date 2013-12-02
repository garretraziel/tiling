#include "neuron.hpp"

val_t signum(val_t sum) {
    return sum > 0? 1 : -1;
}

val_t Neuron::val() {
    WeightMap::iterator it;
    val_t sum = 0;
    for (it = weights.begin(); it != weights.end(); it++) {
        sum += it->first->val() * it->second;
    }
    return act_func(sum);
}
