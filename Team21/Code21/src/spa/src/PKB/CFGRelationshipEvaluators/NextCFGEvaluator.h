//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTCFGEVALUATOR_H
#define SPA_NEXTCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"

class NextCFGEvaluator : public CFGRelationshipEvaluator {
 public:

    ~NextCFGEvaluator() = default;

    NextCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                   EntityManager* entityManager);

  [[nodiscard]] auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Statement* leftStatement,
                             Statement* rightStatement)
      -> std::shared_ptr<Relationship> override;

  // auto isRelated(std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
  //                Statement& destinationStatement, bool isReverse)
  //     -> bool override;
  auto getRelatedBlockStatementPairs(
      std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
      bool isReverse)
      -> std::shared_ptr<std::vector<
          std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> override;
};

#endif  // SPA_NEXTCFGEVALUATOR_H
