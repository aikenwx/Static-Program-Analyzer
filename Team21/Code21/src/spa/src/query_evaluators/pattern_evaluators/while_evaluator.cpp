#include <unordered_set>

#include "while_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
auto WhileEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<Product> {
  std::vector<Product> while_products;
  const auto &ref1 = getClause().getArg1();

  if (std::holds_alternative<QuotedIdentifier>(ref1)) {
    std::unordered_set<WhileStatement *>
        *set = pkb.getWhileStatementsUsingVariableInCondition(std::get<QuotedIdentifier>(ref1).getQuotedId());
    for (const auto &row : *set) {
      auto product = Product(row);
      while_products.push_back(product);
    }
  } else {
    auto *all_while_variable_pairs =
        pkb.getUsesRelationshipsByLeftAndRightEntityTypes(WhileStatement::getEntityTypeStatic(),
                                                          Variable::getEntityTypeStatic());
    for (const auto &row : *all_while_variable_pairs) {
      auto *rel = dynamic_cast<Relationship *>(row);
      int left = stoi(*rel->getLeftHandEntity()->getEntityValue());
      std::unordered_set<Variable *> *varSet = pkb.getVariablesInWhileStatementCondition(left);
      auto *var = dynamic_cast<Variable *>(row->getRightHandEntity());
      if (varSet->find(var) != varSet->end()) {
        auto product = Product(row);
        while_products.push_back(product);
      }
    }
  }
  return while_products;
}
}  // namespace qps
