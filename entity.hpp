#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

typedef double val_t;

class Entity {
public:
    virtual val_t val() =0;
    virtual ~Entity() {};
};

#endif
