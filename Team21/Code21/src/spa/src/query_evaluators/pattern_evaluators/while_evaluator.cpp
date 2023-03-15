#include "while_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
std::vector<Product> WhileEvaluator::CallPkb(QueryFacade &pkb) {
  std::vector<Product> while_products;
  Ref ref1 = clause_.getArg1();

  if (std::holds_alternative<QuotedIdentifier>(ref1)) {
    std::set<WhileStatement *>
        *set = pkb.getWhileStatementsUsingVariableInCondition(std::get<QuotedIdentifier>(ref1).getQuotedId());
    for (const auto &row : *set) {
      Product p = Product(row);
      while_products.push_back(p);
    }
  } else {
    auto all_while_variable_pairs =
        pkb.getUsesRelationshipsByLeftAndRightEntityTypes(WhileStatement::getEntityTypeStatic(),
                                                          Variable::getEntityTypeStatic());
    for (const auto &row : *all_while_variable_pairs) {
      Product p = Product(row);
      while_products.push_back(p);
    }
  }
  return while_products;
}
} // qps
