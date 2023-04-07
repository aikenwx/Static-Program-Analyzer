//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTCFGEVALUATOR_H
#define SPA_NEXTCFGEVALUATOR_H

#include "NextRelatedCFGEvaluator.h"

class NextCFGEvaluator : public NextRelatedCFGEvaluator {
 public:
    NextCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                   EntityManager* entityManager);

  [[nodiscard]] auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Entity *leftStatement,
                             Entity *rightStatement)
      -> std::unique_ptr<Relationship> override;

  // auto isRelated(std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
  //                Statement& destinationStatement, bool isReverse)
  //     -> bool override;
  auto getRelatedStatementNumbers(
          int sourceStatementNumber,
          bool isReverse)
      -> std::unique_ptr<std::vector<int>> override;
};

#endif  // SPA_NEXTCFGEVALUATOR_H
