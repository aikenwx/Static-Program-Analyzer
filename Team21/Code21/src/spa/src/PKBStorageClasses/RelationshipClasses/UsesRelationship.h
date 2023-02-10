//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_USESRELATIONSHIP_H
#define SPA_USESRELATIONSHIP_H


#include "Relationship.h"
#include "../EntityClasses/Variable.h"
#include <memory>

class UsesRelationship : public Relationship {
private:
    // Can be Statement or Procedure
    std::shared_ptr<Entity> user;
    std::shared_ptr<Variable> usedVariable;

public:
    UsesRelationship(Entity *user, Variable *usedVariable);

    bool containsEntityOnLeftHand(Entity *entity) override;

    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;

    Entity* getLeftHandEntity() override;

    Entity* getRightHandEntity() override;
};


#endif //SPA_USESRELATIONSHIP_H
