//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_USESRELATIONSHIP_H
#define SPA_USESRELATIONSHIP_H


#include "Relationship.h"
#include "../EntityClasses/Variable.h"

class UsesRelationship : public Relationship {
private:
    // Can be Statement or Procedure
    Entity* user;
    Variable* usedVariable;

public:
    UsesRelationship(Entity *user, Variable *usedVariable);

    bool containsEntityOnLeftHand(Entity *entity) override;

    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;

    Entity* getLeftHandEntity() override;

    Entity* getRightHandEntity() override;
};


#endif //SPA_USESRELATIONSHIP_H
