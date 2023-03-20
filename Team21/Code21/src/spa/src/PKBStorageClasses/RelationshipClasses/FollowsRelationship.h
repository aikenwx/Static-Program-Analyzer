//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_FOLLOWSRELATIONSHIP_H
#define SPA_FOLLOWSRELATIONSHIP_H

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class FollowsRelationship : public Relationship {
   private:
    static const RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> const RelationshipType &;
    FollowsRelationship(Statement* followedStatement, Statement* followerStatement);
    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;
};

#endif  // SPA_FOLLOWSRELATIONSHIP_H
