#include <unordered_set>

#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "if_evaluator.h"
#include "query_evaluators/pattern_evaluators/pattern_evaluator.h"

namespace qps {
auto IfEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<Product> {
  std::vector<Product> if_products;
  Ref ref1 = getClause().getArg1();

  if (std::holds_alternative<QuotedIdentifier>(ref1)) {
    std::unordered_set<IfStatement *>
        *set = pkb.getIfStatementsUsingVariableInCondition(std::get<QuotedIdentifier>(ref1).getQuotedId());
    for (const auto &row : *set) {
      auto product = Product(row);
      if_products.push_back(product);
    }
  } else {
    auto *all_if_variable_pairs =
        pkb.getUsesRelationshipsByLeftAndRightEntityTypes(IfStatement::getEntityTypeStatic(),
                                                          Variable::getEntityTypeStatic());
    for (const auto &row : *all_if_variable_pairs) {
      auto* rel = dynamic_cast<Relationship*>(row);
      int left = stoi(*rel->getLeftHandEntity()->getEntityValue());
      std::unordered_set<Variable *>* varSet = pkb.getVariablesInIfStatementCondition(left);
      auto* var = dynamic_cast<Variable*>(row->getRightHandEntity());
      if (varSet->find(var) != varSet->end()) {
        auto product = Product(row);
        if_products.push_back(product);
      }
    }
  }
  return if_products;
}
}  // namespace qps
