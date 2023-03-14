#include "pattern_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {

PatternEvaluator::PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

ClauseEvaluator::ClauseResult PatternEvaluator::Evaluate(QueryFacade &pkb) {
  return qps::PatternEvaluator::ConstructResult(CallPkb(pkb));
}

ClauseEvaluator::ClauseResult PatternEvaluator::ConstructResult(const std::vector<Product> &statements) {
  std::vector<Synonym> syns;
  syns.push_back(clause_.getStmtSynonym());

  bool lhs_syn = false;
  Ref ref = clause_.getArg1();
  if (Synonym *syn = std::get_if<Synonym>(&ref)) {
    syns.push_back(*syn);
    lhs_syn = true;
  }

  SynonymTable table(syns);
  for (int i = 0; i < statements.size(); i++) {
    Product prod = statements.at(i);
    std::size_t ind = prod.index();
    if (std::holds_alternative<ModifiesRelationship>(prod)) {
      auto mod = std::get_if<ModifiesRelationship>(&prod);
      SynonymTable::Row row = { *mod->getLeftHandEntity()->getEntityValue() };
      if (lhs_syn) row.push_back(*mod->getRightHandEntity()->getEntityValue());
      table.AddRow(row);
    }
    else if (std::holds_alternative<UsesRelationship>(prod)){
      auto use = std::get_if<UsesRelationship>(&prod);
      SynonymTable::Row row = { *use->getLeftHandEntity()->getEntityValue() };
      if (lhs_syn) row.push_back(*use->getRightHandEntity()->getEntityValue());
      table.AddRow(row);
    }
    else if (std::holds_alternative<IfStatement>(prod)) {
      auto i = std::get_if<IfStatement>(&prod);
      SynonymTable::Row row = { *i->getEntityValue() };
      table.AddRow(row);
    }
    else if (std::holds_alternative<WhileStatement>(prod)) {
      auto w = std::get_if<WhileStatement>(&prod);
      SynonymTable::Row row = { *w->getEntityValue() };
      table.AddRow(row);
    }
  }
  return table;
}
} // qps
