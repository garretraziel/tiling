#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <vector>

typedef double val_t;

class Entity;
typedef std::vector<Entity*> EntityVector;

class Entity {
public:
    virtual val_t val() =0;
    virtual ~Entity() {};
};

#endif
