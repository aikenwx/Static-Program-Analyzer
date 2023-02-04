//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_MODIFIESRELATIONSHIP_H
#define SPA_MODIFIESRELATIONSHIP_H

#include "Relationship.h"
#include "../EntityClasses/Variable.h"

class ModifiesRelationship : public Relationship {
private:
    // Can be Statement or Procedure
    Entity *modifier;
    Variable *modifiedVariable;

public:
    ModifiesRelationship(Entity *modifier, Variable *modifiedVariable);

    bool containsEntityOnLeftHand(Entity *entity) override;

    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;
};


#endif //SPA_MODIFIESRELATIONSHIP_H
