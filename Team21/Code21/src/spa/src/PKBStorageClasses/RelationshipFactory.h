//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_RELATIONSHIPFACTORY_H
#define SPA_RELATIONSHIPFACTORY_H

#include "RelationshipClasses/ModifiesRelationship.h"
#include "RelationshipClasses/UsesRelationship.h"
#include "RelationshipClasses/ParentRelationship.h"
#include "RelationshipClasses/FollowsRelationship.h"
#include "EntityClasses/Entity.h"

enum RelationshipType {
    MODIFIES = 0,
    USES = 1,
    PARENT = 2,
    FOLLOWS = 3,
};

class RelationshipFactory {
public:
    RelationshipFactory();
    Relationship* createRelationship(RelationshipType relationshipType, Entity* firstEntity, Entity* secondEntity);
};


#endif //SPA_RELATIONSHIPFACTORY_H
