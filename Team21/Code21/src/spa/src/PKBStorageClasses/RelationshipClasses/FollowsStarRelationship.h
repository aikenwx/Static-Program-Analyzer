//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_FOLLOWSSTARRELATIONSHIP_H
#define SPA_FOLLOWSSTARRELATIONSHIP_H

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class FollowsStarRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> RelationshipType &;
    FollowsStarRelationship(Statement *followedStatement, Statement *followerStatement);
    [[nodiscard]] auto getRelationshipType() const
        -> RelationshipType & override;
};

#endif  // SPA_FOLLOWSSTARRELATIONSHIP_H
