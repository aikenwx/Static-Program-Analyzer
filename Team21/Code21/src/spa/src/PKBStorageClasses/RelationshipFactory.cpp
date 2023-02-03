//
// Created by Aiken Wong on 3/2/23.
//

#include "RelationshipFactory.h"


Relationship* RelationshipFactory::createRelationship(RelationshipType relationshipType, Entity* firstEntity, Entity* secondEntity) {
    switch (relationshipType) {
        case MODIFIES:
            if (dynamic_cast<Variable*>(secondEntity) == nullptr) {
                throw("Variable expected for second entity of Modifies Relationship");
            }
            return new ModifiesRelationship(firstEntity, (Variable*) secondEntity);
        case USES:
            if (dynamic_cast<Variable*>(secondEntity) == nullptr) {
                throw("Variable expected for second entity of Uses Relationship");
            }
            return new UsesRelationship(firstEntity, (Variable*) secondEntity);
        case PARENT:
            if (dynamic_cast<Statement*>(firstEntity) == nullptr || dynamic_cast<Statement*>(secondEntity) == nullptr) {
                throw("Statement expected for both entities of Parent Relationship");
            }
            return new ParentRelationship((Statement*) firstEntity, (Statement*) secondEntity);
        case FOLLOWS:
            if (dynamic_cast<Statement*>(firstEntity) == nullptr || dynamic_cast<Statement*>(secondEntity) == nullptr) {
                throw("Statement expected for both entities of Follows Relationship");
            }
            return new FollowsRelationship((Statement*) firstEntity, (Statement*) secondEntity);
        default:
            throw("Invalid Relationship Type");
    }
}

RelationshipFactory::RelationshipFactory() {};
