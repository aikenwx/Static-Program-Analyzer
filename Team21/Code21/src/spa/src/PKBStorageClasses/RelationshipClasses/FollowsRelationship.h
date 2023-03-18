//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_FOLLOWSRELATIONSHIP_H
#define SPA_FOLLOWSRELATIONSHIP_H

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class FollowsRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> RelationshipType&;
    FollowsRelationship(Statement* followedStatement, Statement* followerStatement);
    [[nodiscard]] auto getRelationshipType() const
        -> RelationshipType& override;
};

#endif  // SPA_FOLLOWSRELATIONSHIP_H
