//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_MODIFIESRELATIONSHIP_H
#define SPA_MODIFIESRELATIONSHIP_H

#include "Relationship.h"
#include "../EntityClasses/Variable.h"
#include <memory>

class ModifiesRelationship : public Relationship {
private:
    // Can be Statement or Procedure

    std::shared_ptr<Entity> modifier;
    std::shared_ptr<Variable> modifiedVariable;

public:
    ModifiesRelationship(Entity *modifier, Variable *modifiedVariable);

    bool containsEntityOnLeftHand(Entity *entity) override;

    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;

    Entity* getLeftHandEntity() override;

    Entity* getRightHandEntity() override;

};


#endif //SPA_MODIFIESRELATIONSHIP_H
