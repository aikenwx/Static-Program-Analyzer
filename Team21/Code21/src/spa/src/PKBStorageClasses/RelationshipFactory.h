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

class RelationshipFactory {
public:
    RelationshipFactory();
    Relationship* createRelationship(RelationshipType relationshipType, Entity* firstEntity, Entity* secondEntity);
};


#endif //SPA_RELATIONSHIPFACTORY_H
