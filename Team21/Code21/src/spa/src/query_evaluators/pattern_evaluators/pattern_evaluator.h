#pragma once

#include "query/declaration.h"
#include "query/pattern_clause.h"
#include "query_evaluators/clause_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "product.h"

namespace qps {

class PatternEvaluator : public ClauseEvaluator {
 public:
  PatternEvaluator(const PatternClause &clause, const std::vector<Declaration> &declarations);

  auto getClause() -> const PatternClause &;

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;

  virtual auto CallPkb(QueryFacade &pkb) -> std::vector<Product> = 0;

 private:
  const PatternClause &clause_;

  auto ConstructResult(const std::vector<Product> &) -> ClauseResult;

  const std::vector<Declaration> &declarations_;

};

}  // namespace qps
