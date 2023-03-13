//
// Created by Aiken Wong on 3/3/23.
//

#ifndef INC_22S2_CP_SPA_TEAM_21_CALLSSTARRELATIONSHIP_H
#define INC_22S2_CP_SPA_TEAM_21_CALLSSTARRELATIONSHIP_H

#include "../EntityClasses/Procedure.h"
#include "Relationship.h"

class CallsStarRelationship : public Relationship {
   private:
    static RelationshipType relationshipType;

   public:
    CallsStarRelationship(Procedure* caller, Procedure* callee);

    static RelationshipType& getRelationshipTypeStatic();

    RelationshipType& getRelationshipType() const override;
};

#endif  // INC_22S2_CP_SPA_TEAM_21_CALLSSTARRELATIONSHIP_H
