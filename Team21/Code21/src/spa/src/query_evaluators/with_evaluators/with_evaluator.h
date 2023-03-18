#pragma once

#include "query/declaration.h"
#include "query/with_clause.h"
#include "query_evaluators/clause_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

class WithEvaluator : public ClauseEvaluator {
 public:
  WithEvaluator(WithClause clause, std::vector<Declaration> declarations);

  auto getClause() -> WithClause;

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;

  virtual auto CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>*> = 0;

 private:
  WithClause clause_;

  //nullPtr means false result while emppty vector means true result
  auto ConstructResult(const std::vector<std::vector<Entity *>*> &) -> ClauseResult;

  std::vector<Declaration> declarations_;

};

}  // namespace qps
