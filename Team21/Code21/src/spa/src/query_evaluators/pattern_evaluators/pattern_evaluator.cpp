#include "pattern_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/pattern_evaluators/pattern_evaluator.h"

namespace qps {

PatternEvaluator::PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

auto PatternEvaluator::getClause() -> PatternClause {
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
    if (std::holds_alternative<ModifiesRelationship *>(prod)) {
      auto *mod = std::get<ModifiesRelationship *>(prod);
      SynonymTable::Row row = {mod->getLeftHandEntity()};
      if (lhs_syn) { row.push_back(mod->getRightHandEntity()); }
      table.AddRow(row);
    } else if (std::holds_alternative<UsesRelationship *>(prod)) {
      auto *use = std::get<UsesRelationship *>(prod);
      SynonymTable::Row row = {use->getLeftHandEntity()};
      if (lhs_syn) { row.push_back(use->getRightHandEntity()); }
      table.AddRow(row);
    } else if (std::holds_alternative<IfStatement *>(prod)) {
      auto *ifs = std::get<IfStatement *>(prod);
      SynonymTable::Row row = {ifs};
      table.AddRow(row);
    } else if (std::holds_alternative<WhileStatement *>(prod)) {
      auto *whiles = std::get<WhileStatement *>(prod);
      SynonymTable::Row row = {whiles};
      table.AddRow(row);
    }
  }
  return table;
}
}  // namespace qps
