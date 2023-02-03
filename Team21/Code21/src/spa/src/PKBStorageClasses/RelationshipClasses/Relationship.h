#ifndef SPA_RELATIONSHIP_H
#define SPA_RELATIONSHIP_H

#include "../EntityClasses/AssignStatement.h"
#include "../EntityClasses/Procedure.h"

class Relationship {
public:
    virtual bool containsEntityOnLeftHand(Entity *entity) = 0;

    virtual bool containsEntityOnRightHand(Entity *entity) = 0;
};


#endif //SPA_RELATIONSHIP_H
