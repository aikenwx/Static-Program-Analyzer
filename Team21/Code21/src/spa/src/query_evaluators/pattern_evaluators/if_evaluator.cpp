#include <unordered_set>

#include "if_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
std::vector<Product> IfEvaluator::CallPkb(QueryFacade &pkb) {
  std::vector<Product> if_products;
  Ref ref1 = clause_.getArg1();

  if (std::holds_alternative<QuotedIdentifier>(ref1)) {
    std::unordered_set<IfStatement *>
        *set = pkb.getIfStatementsUsingVariableInCondition(std::get<QuotedIdentifier>(ref1).getQuotedId());
    for (const auto &row : *set) {
      Product p = Product(row);
      if_products.push_back(p);
    }
  } else {
    auto all_if_variable_pairs =
        pkb.getUsesRelationshipsByLeftAndRightEntityTypes(IfStatement::getEntityTypeStatic(),
                                                          Variable::getEntityTypeStatic());
    for (const auto &row : *all_if_variable_pairs) {
      Product p = Product(row);
      if_products.push_back(p);
    }
  }
  return if_products;
}
} // qps
