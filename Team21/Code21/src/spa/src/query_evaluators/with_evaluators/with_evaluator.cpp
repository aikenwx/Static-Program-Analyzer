#include "with_evaluator.h"
#include "query/attr_ref.h"
#include "query/ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/with_evaluators/with_evaluator.h"

namespace qps {

WithEvaluator::WithEvaluator(WithClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

auto WithEvaluator::getClause() -> WithClause {
  return clause_;
}

auto WithEvaluator::getDeclarations() -> std::vector<Declaration> {
  return declarations_;
}

auto WithEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  return qps::WithEvaluator::ConstructResult(CallPkb(pkb));
}

auto WithEvaluator::ConstructResult(const std::vector<std::vector<Entity *>*> &statements) -> ClauseResult {
  std::vector<Synonym> syns;

  WithRefType  ref1 = clause_.getRef1().ref;
  WithRefType  ref2 = clause_.getRef2().ref;

  Synonym *syn1 = nullptr;
  Synonym *syn2 = nullptr;

  AttrRef *attr1 = std::get_if<AttrRef>(&ref1);
  AttrRef *attr2 = std::get_if<AttrRef>(&ref2);

  if (attr1 != nullptr) {
    *syn1 = attr1->synonym;
    syns.push_back(*syn1);
  }

  if (attr2 != nullptr) {
    *syn2 = attr2->synonym;
    syns.push_back(*syn2);
  }

  SynonymTable table(syns);

  if (statements.empty()) {
    return false;
  }

  if (statements.size() == 1 && statements[0]->empty()) {
    return true;
  }

  if (statements.size() == 1) {
    SynonymTable::Row row;
    for (auto *entity : *statements[0]) {
      row = {};
      row.push_back(entity);
      table.AddRow(row);
    }
  }
  else if (statements.size() == 2) {
    SynonymTable::Row row;
    for (int num = 0; num < statements[0]->size(); num++) {
      row = {};
      row.push_back(statements[0]->at(num));
      row.push_back(statements[1]->at(num));
      table.AddRow(row);
    }
  }

  return table;
}
}  // namespace qps
