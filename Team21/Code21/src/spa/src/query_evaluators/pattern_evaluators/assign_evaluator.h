#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class AssignEvaluator : public PatternEvaluator {
 public:
  AssignEvaluator(PatternClause clause, std::vector<Declaration> declarations)
      : PatternEvaluator(std::move(clause), std::move(declarations)) {}
  std::vector<Relationship *> CallPkb(QueryFacade &pkb) override;

  int postfixHelper(char a);

  std::string makePostfix(std::string str);

  std::vector<ModifiesRelationship *> checkExpressionContained(std::vector<ModifiesRelationship *> pkb_res,
                                                               std::string postfix,
                                                               bool is_partial);
};

} // qps
