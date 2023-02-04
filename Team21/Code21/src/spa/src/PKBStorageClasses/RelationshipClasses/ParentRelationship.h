//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_PARENTRELATIONSHIP_H
#define SPA_PARENTRELATIONSHIP_H

#include "Relationship.h"
#include "../EntityClasses/Statement.h"

class ParentRelationship : public Relationship {
private:
    Statement *parentStatement;
    Statement *childStatement;

public:
    ParentRelationship(Statement *parentStatement, Statement *childStatement);
    bool containsEntityOnLeftHand(Entity *entity) override;
    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;
};


#endif //SPA_PARENTRELATIONSHIP_H
