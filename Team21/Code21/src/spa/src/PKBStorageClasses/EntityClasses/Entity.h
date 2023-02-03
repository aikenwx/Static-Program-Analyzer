//
// Created by Aiken Wong on 1/2/23.
//

#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>

//enum EntityType {
//    ASSIGN = 0, READ = 1, PRINT = 2, CALL = 3, WHILE = 4, IF = 5, VARIABLE = 6, PROCEDURE = 7, CONSTANT = 8
//};

class Entity {

public:
    virtual std::string getEntityValue() = 0;

    virtual bool equals(Entity *otherEntity) = 0;

    virtual ~Entity() = default;
};


#endif
