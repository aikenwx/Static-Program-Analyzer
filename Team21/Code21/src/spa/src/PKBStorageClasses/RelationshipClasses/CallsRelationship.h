//
// Created by Aiken Wong on 3/3/23.
//

#ifndef INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
#define INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H

#include "Relationship.h"
#include "../EntityClasses/Procedure.h"

class CallsRelationship : public Relationship {
private:
    Procedure* caller;
    Procedure* callee;

public:
    CallsRelationship(Procedure* caller, Procedure* callee);

    bool containsEntityOnLeftHand(Entity *entity) override;

    bool containsEntityOnRightHand(Entity *entity) override;

    RelationshipType getRelationshipType() override;

    Entity* getLeftHandEntity() override;

    Entity* getRightHandEntity() override;
};



#endif //INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
