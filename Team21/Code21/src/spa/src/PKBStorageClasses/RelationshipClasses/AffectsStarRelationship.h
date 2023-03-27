//
// Created by Aiken Wong on 27/3/23.
//

#ifndef SPA_AFFECTSSTARRELATIONSHIP_H
#define SPA_AFFECTSSTARRELATIONSHIP_H


#include "Relationship.h"
class AffectsStarRelationship : public Relationship {
   private:
    static const RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> const RelationshipType &;

    AffectsStarRelationship(AssignStatement* affecter, AssignStatement* affectee);

    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;
};


#endif //SPA_AFFECTSSTARRELATIONSHIP_H
