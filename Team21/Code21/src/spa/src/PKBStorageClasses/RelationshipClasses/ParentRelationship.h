//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_PARENTRELATIONSHIP_H
#define SPA_PARENTRELATIONSHIP_H

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class ParentRelationship : public Relationship {
   private:
    static const RelationshipType relationshipType;

   public:
    ParentRelationship(Statement *parentStatement, Statement *childStatement);

    static auto getRelationshipTypeStatic() -> const RelationshipType &;

    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;
};

#endif  // SPA_PARENTRELATIONSHIP_H
