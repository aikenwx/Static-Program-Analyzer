//
// Created by Aiken Wong on 9/4/23.
//

#ifndef SPA_USESINPARENTCONDITIONRELATIONSHIP_H
#define SPA_USESINPARENTCONDITIONRELATIONSHIP_H

#include "PKBStorageClasses/EntityClasses/ParentStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "Relationship.h"

class UsesInParentConditionRelationship : public Relationship {
 private:
  static const RelationshipType relationshipType;

 public:
  static auto getRelationshipTypeStatic() -> const RelationshipType &;

  UsesInParentConditionRelationship(ParentStatement *user, Variable *usedVariable);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType & override;
};

#endif  // SPA_USESINPARENTCONDITIONRELATIONSHIP_H
