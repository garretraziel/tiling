#ifndef __CONSTANT_HPP__
#define __CONSTANT_HPP__

#include "entity.hpp"
#include "testset.hpp"
#include <vector>

// trida, reprezentujici
// konstantu - napriklad
// neuron vstupni vrstvy, nebo
// bias
class Constant;

typedef std::vector<Constant*> ConstantVector;

class Constant : public Entity {
public:
    Constant():priv_value(0) {}
    Constant(val_t value):priv_value(value) {}

    // nastavi hodnotu dane konstanty
    val_t set_val(val_t value) {priv_value=value; return priv_value;}
    val_t val() {return priv_value;}
private:
    val_t priv_value;
};

// trida, reprezentujici mnozinu
// vstupnich neuronu
class Inputs {
public:
    Inputs(int length);
    ~Inputs();

    // nastavi postupne kazdemu objektu
    // prislusnou hodnotu vstupu
    bool set_values(InputVector values);
    ConstantVector input_vector;
    // prevede na typ EntityVector
    EntityVector as_entities();
};

#endif
