//
// Created by Aiken Wong on 2/2/23.
//
#include <string>
#include "Entity.h"

#ifndef SPA_CONSTANT_H
#define SPA_CONSTANT_H


class Constant : public Entity {
private:
    int constantValue;

public:
    Constant(int constantValue);

    std::string getEntityValue();

    bool equals(Entity* otherEntity);
};


#endif //SPA_CONSTANT_H
