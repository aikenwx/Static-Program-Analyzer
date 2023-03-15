//
// Created by Aiken Wong on 3/3/23.
//

#ifndef INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
#define INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H

#include "../EntityClasses/Procedure.h"
#include "Relationship.h"

class CallsRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    static RelationshipType& getRelationshipTypeStatic();

    CallsRelationship(Procedure* caller, Procedure* callee);

    RelationshipType& getRelationshipType() const override;
};

#endif  // INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
