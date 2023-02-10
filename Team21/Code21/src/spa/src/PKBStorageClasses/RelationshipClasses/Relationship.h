#ifndef SPA_RELATIONSHIP_H
#define SPA_RELATIONSHIP_H

#include "../EntityClasses/AssignStatement.h"
#include "../EntityClasses/Procedure.h"

enum RelationshipType {
    MODIFIES = 0,
    USES = 1,
    PARENT = 2,
    FOLLOWS = 3,
    LAST_RELATIONSHIP = FOLLOWS,
};

class Relationship {
public:
    virtual ~Relationship() = default;

    virtual bool containsEntityOnLeftHand(Entity *entity) = 0;

    virtual bool containsEntityOnRightHand(Entity *entity) = 0;

    virtual RelationshipType getRelationshipType() = 0;

    virtual Entity *getLeftHandEntity() = 0;

    virtual Entity *getRightHandEntity() = 0;
};


#endif //SPA_RELATIONSHIP_H
