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
  PatternClause clause_;
  PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations);

  ClauseResult Evaluate(QueryFacade &pkb) override;

  virtual std::vector<Product> CallPkb(QueryFacade &pkb) = 0;

 private:

  ClauseResult ConstructResult(const std::vector<Product> &);

  std::vector<Declaration> declarations_;

};

} // qps
