//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYFACTORY_H
#define SPA_ENTITYFACTORY_H

#include "EntityClasses/Entity.h"
#include <string>

enum InputType {
    STRING = 0,
    INTEGER = 1,
};

class EntityFactory {
public:
    EntityFactory();
    Entity* createEntity(EntityType entityType, std::string entityValue);
    Entity* createEntity(EntityType entityType, int entityValue);

private:
    void checkForValidInput(EntityType entityType, InputType inputType);
};




#endif //SPA_ENTITYFACTORY_H
