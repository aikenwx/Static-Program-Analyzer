//
// Created by Aiken Wong on 1/2/23.
//

#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>
#include <vector>

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
    STATEMENT = 9,
    LAST_ENTITY = STATEMENT
};

class Entity {
   public:
    static std::vector<EntityType> statementTypes;

   public:
    virtual ~Entity(){};

    virtual std::string* getEntityValue() = 0;

    virtual EntityType getEntityType() = 0;

    bool equals(Entity* otherEntity);

    static bool isStatementType(EntityType entityType);
};

#endif
