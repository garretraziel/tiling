#ifndef __CONSTANT_HPP__
#define __CONSTANT_HPP__

#include "entity.hpp"
#include <vector>

class Constant;

typedef std::vector<Constant*> PConstantVector;

class Constant : public Entity {
public:
    Constant():priv_value(0) {}
    Constant(val_t value):priv_value(value) {}

    val_t set_val(val_t value) {priv_value=value; return priv_value;}
    val_t val() {return priv_value;}
private:
    val_t priv_value;
};

#endif
