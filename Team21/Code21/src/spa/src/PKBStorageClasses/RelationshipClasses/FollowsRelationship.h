//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_FOLLOWSRELATIONSHIP_H
#define SPA_FOLLOWSRELATIONSHIP_H


#include "Relationship.h"
#include "../EntityClasses/Statement.h"

class FollowsRelationship : public Relationship {
private:
    Statement *followedStatement;
    Statement *followerStatement;

public:
    FollowsRelationship(Statement *followedStatement, Statement *followerStatement);
    bool containsEntityOnLeftHand(Entity *entity) override;
    bool containsEntityOnRightHand(Entity *entity) override;
    RelationshipType getRelationshipType() override;
    Entity* getLeftHandEntity() override;
    Entity* getRightHandEntity() override;
};


#endif //SPA_FOLLOWSRELATIONSHIP_H
