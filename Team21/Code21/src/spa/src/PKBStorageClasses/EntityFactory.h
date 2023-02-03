//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYFACTORY_H
#define SPA_ENTITYFACTORY_H

#include "EntityClasses/Entity.h"
#include <string>

enum EntityType {
    ASSIGN_STATEMENT = 0,
    CALL_STATEMENT = 1,
    IF_STATEMENT = 2,
    PRINT_STATEMENT = 3,
    READ_STATEMENT = 4,
    WHILE_STATEMENT = 5,
    CONSTANT = 6,
    VARIABLE = 7,
    PROCEDURE = 8,
};

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
    bool isStatement(EntityType entityType);
    void checkForValidInput(EntityType entityType, InputType inputType);
};




#endif //SPA_ENTITYFACTORY_H
