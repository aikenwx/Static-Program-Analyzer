//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_MODIFIESRELATIONSHIP_H
#define SPA_MODIFIESRELATIONSHIP_H

#include "../EntityClasses/Variable.h"
#include "Relationship.h"

class ModifiesRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    ModifiesRelationship(Entity* modifier, Variable* modifiedVariable);

    static auto getRelationshipTypeStatic() -> RelationshipType&;

    [[nodiscard]] auto getRelationshipType() const
        -> RelationshipType& override;
};

#endif  // SPA_MODIFIESRELATIONSHIP_H
