#include "constant.hpp"

Inputs::Inputs(int length) {
    for (int i = 0; i < length; i++) {
        Constant *c = new Constant();
        input_vector.push_back(c);
    }
}

Inputs::~Inputs() {
    PConstantVector::iterator pcit;
    for (pcit = input_vector.begin(); pcit != input_vector.end(); pcit++) {
        delete *pcit;
    }
}

bool Inputs::set_values(InputVector values) {
    if (values.size() != input_vector.size()) {
        return false;
    }
    int size = input_vector.size();
    for (int i = 0; i < size; i++) {
        input_vector[i]->set_val(values[i]);
    }
    return true;
}
