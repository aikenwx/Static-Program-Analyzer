#include "pattern_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {

PatternEvaluator::PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

ClauseEvaluator::ClauseResult PatternEvaluator::Evaluate(QueryFacade &pkb) {
  return qps::PatternEvaluator::ConstructResult(CallPkb(pkb));
}

ClauseEvaluator::ClauseResult PatternEvaluator::ConstructResult(const std::vector<Relationship *> &statements) {
  std::vector<Synonym> syns;
  syns.push_back(clause_.getAssign());

  bool lhs_syn = false;
  Ref ref = clause_.getArg1();
  if (Synonym *syn = std::get_if<Synonym>(&ref)) {
    syns.push_back(*syn);
    lhs_syn = true;
  }

  SynonymTable table(syns);
  for (auto ans : statements) {
    SynonymTable::Row row = {*ans->getLeftHandEntity()->getEntityValue()};
    if (lhs_syn) row.push_back(*ans->getRightHandEntity()->getEntityValue());
    table.AddRow(row);
  }
  return table;
}
} // qps
