//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_PARENTRELATIONSHIP_H
#define SPA_PARENTRELATIONSHIP_H

#include <memory>

#include "../EntityClasses/Statement.h"
#include "Relationship.h"

class ParentRelationship : public Relationship {
   private:
    std::shared_ptr<Statement> parentStatement;
    std::shared_ptr<Statement> childStatement;

   public:
    ParentRelationship(Statement *parentStatement, Statement *childStatement);
    bool containsEntityOnLeftHand(Entity *entity) override;
    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;

    Entity *getLeftHandEntity() override;

    Entity *getRightHandEntity() override;
};

#endif  // SPA_PARENTRELATIONSHIP_H
