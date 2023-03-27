//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTCFGEVALUATOR_H
#define SPA_NEXTCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"
class NextCFGEvaluator : public CFGRelationshipEvaluator {
 public:
  NextCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager);

     auto getRelationshipType() const -> const RelationshipType & override;

    auto evaluateAndCacheRelationshipFromLeftStatement(Statement* leftStatement) -> std::vector<Relationship*>* override;

  auto evaluateAndCacheRelationshipFromRightStatement(Statement* rightEntity) -> std::vector<Relationship*>* override;


};

#endif  // SPA_NEXTCFGEVALUATOR_H
