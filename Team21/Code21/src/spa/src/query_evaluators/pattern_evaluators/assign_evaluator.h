#pragma once

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"
#include "PKB/QueryFacade.h"

namespace qps {

class AssignEvaluator : public PatternEvaluator {
 public:
  AssignEvaluator(PatternClause clause, std::vector<Declaration> declarations)
      : PatternEvaluator(std::move(clause), std::move(declarations)) {}
  std::vector<Entity *> CallPkb(QueryFacade &pkb) override;

  int postfixHelper(char a);

  std::string makePostfix(std::string str);

  std::vector<Entity *> checkExpressionContained(std::vector<Entity *> pkb_res,
                                                 std::string postfix,
                                                 bool is_partial);
};

} // qps
