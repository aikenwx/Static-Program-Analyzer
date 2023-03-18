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
    static auto getRelationshipTypeStatic() -> RelationshipType &;

    UsesRelationship(Entity *user, Variable *usedVariable);

    [[nodiscard]] auto getRelationshipType() const
        -> RelationshipType & override;
};

#endif  // SPA_USESRELATIONSHIP_H
