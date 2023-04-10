//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_USESRELATIONSHIP_H
#define SPA_USESRELATIONSHIP_H

#include "../EntityClasses/Variable.h"
#include "Relationship.h"

class UsesRelationship : public Relationship {
 private:
  static const RelationshipType relationshipType;

 public:
  static auto getRelationshipTypeStatic() -> const RelationshipType &;

  UsesRelationship(Procedure *user, Variable *usedVariable);

  UsesRelationship(Statement *user, Variable *usedVariable);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType & override;
};

#endif  // SPA_USESRELATIONSHIP_H
