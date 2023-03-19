//
// Created by Aiken Wong on 3/3/23.
//

#ifndef INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
#define INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H

#include "../EntityClasses/Procedure.h"
#include "Relationship.h"

class CallsRelationship : public Relationship {
   private:
    static const RelationshipType relationshipType;

   public:
    static auto getRelationshipTypeStatic() -> const RelationshipType &;

    CallsRelationship(Procedure* caller, Procedure* callee);

    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;
};

#endif  // INC_22S2_CP_SPA_TEAM_21_CALLSRELATIONSHIP_H
