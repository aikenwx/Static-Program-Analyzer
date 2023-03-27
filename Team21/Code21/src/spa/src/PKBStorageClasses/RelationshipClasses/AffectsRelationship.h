//
// Created by Aiken Wong on 27/3/23.
//

#ifndef SPA_AFFECTSRELATIONSHIP_H
#define SPA_AFFECTSRELATIONSHIP_H

#include "Relationship.h"
class AffectsRelationship : public Relationship {
  
   private:
    static const RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> const RelationshipType &;

    AffectsRelationship(AssignStatement* affecter, AssignStatement* affectee);

    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;

};


#endif //SPA_AFFECTSRELATIONSHIP_H
