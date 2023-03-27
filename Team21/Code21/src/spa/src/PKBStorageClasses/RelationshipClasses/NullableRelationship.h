//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NULLABLERELATIONSHIP_H
#define SPA_NULLABLERELATIONSHIP_H

#include "Relationship.h"

class NullableRelationship : public Relationship {
    private:

    static const NullableRelationship nullableRelationship;
    static const RelationshipType relationshipType;
    NullableRelationship();


   public:

    static auto getNullableRelationship() -> const NullableRelationship &;
    
    static auto getRelationshipTypeStatic() -> const RelationshipType &;

    [[nodiscard]] auto getRelationshipType() const
        -> const RelationshipType & override;
};


#endif //SPA_NULLABLERELATIONSHIP_H
