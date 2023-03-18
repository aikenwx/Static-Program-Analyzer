//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_PARENTSTARRELATIONSHIP_H
#define SPA_PARENTSTARRELATIONSHIP_H

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class ParentStarRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    ParentStarRelationship(Statement *parentStatement, Statement *childStatement);

    static auto getRelationshipTypeStatic() -> RelationshipType &;

    [[nodiscard]] auto getRelationshipType() const
        -> RelationshipType & override;
};

#endif  // SPA_PARENTSTARRELATIONSHIP_H
