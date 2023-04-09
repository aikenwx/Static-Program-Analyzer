#include "pattern_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {

PatternEvaluator::PatternEvaluator(const PatternClause &clause, const std::vector<Declaration> &declarations)
    : clause_(clause), declarations_(declarations) {}

auto PatternEvaluator::getClause() -> const PatternClause & {
  return clause_;
}

auto PatternEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  return qps::PatternEvaluator::ConstructResult(CallPkb(pkb));
}

auto PatternEvaluator::ConstructResult(const std::vector<Product> &statements) -> ClauseResult {
  std::vector<Synonym> syns;
  syns.push_back(clause_.getStmtSynonym());

  bool lhs_syn = false;
  Ref ref = clause_.getArg1();
  if (Synonym *syn = std::get_if<Synonym>(&ref)) {
    syns.push_back(*syn);
    lhs_syn = true;
  }

  SynonymTable table(syns);
  for (auto prod : statements) {
    if (std::holds_alternative<Relationship *>(prod)) {
      auto *relationship = std::get<Relationship *>(prod);
      SynonymTable::Row row = {relationship->getLeftHandEntity()};
      if (lhs_syn) { row.push_back(relationship->getRightHandEntity()); }
      table.AddRow(row);
    } else if (std::holds_alternative<Entity *>(prod)) {
      auto *entity = std::get<Entity *>(prod);
      SynonymTable::Row row = {entity};
      table.AddRow(row);
    }
  }
  return table;
}
}  // namespace qps
