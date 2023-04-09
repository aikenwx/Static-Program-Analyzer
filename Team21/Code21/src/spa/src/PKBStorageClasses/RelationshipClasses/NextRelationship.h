//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTRELATIONSHIP_H
#define SPA_NEXTRELATIONSHIP_H

#include "Relationship.h"
#include "CFGEvaluatableRelationshipType.h"
#include "PKB/CFGRelationshipEvaluators/NextCFGEvaluator.h"


class NextRelationshipType : public CFGEvaluableRelationshipType {
  public:
    auto getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                  RelationshipCache *relationshipCache,
                                  EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> const override;
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
