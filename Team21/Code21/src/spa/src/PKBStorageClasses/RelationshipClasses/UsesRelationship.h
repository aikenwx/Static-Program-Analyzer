//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_USESRELATIONSHIP_H
#define SPA_USESRELATIONSHIP_H

#include "../EntityClasses/Variable.h"
#include "Relationship.h"

class UsesRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    static RelationshipType &getRelationshipTypeStatic();

    UsesRelationship(Entity *user, Variable *usedVariable);

    RelationshipType &getRelationshipType() const override;
};

#endif  // SPA_USESRELATIONSHIP_H
