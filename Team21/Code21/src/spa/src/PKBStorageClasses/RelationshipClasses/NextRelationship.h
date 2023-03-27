//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTRELATIONSHIP_H
#define SPA_NEXTRELATIONSHIP_H

#include "Relationship.h"
#include "CFGEvaluatableRelationshipType.h"
#include "PKB/CFGRelationshipEvaluators/NextCFGEvaluator.h"


class NextRelationshipType : public CFGEvaluatableRelationshipType {
  public:
    auto getRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> override;
};

class NextRelationship : public Relationship {
 private:
  static const NextRelationshipType relationshipType;

 public:
  static auto getRelationshipTypeStatic() -> const RelationshipType &;

  NextRelationship(Statement *prevStatement, Statement *nextStatement);

  [[nodiscard]] auto getRelationshipType() const -> const RelationshipType & override;
};

#endif  // SPA_NEXTRELATIONSHIP_H
