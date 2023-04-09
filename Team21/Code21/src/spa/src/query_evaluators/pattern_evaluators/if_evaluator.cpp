#include "if_evaluator.h"

#include <unordered_set>

#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesInParentConditionRelationship.h"
#include "query_evaluators/pattern_evaluators/pattern_evaluator.h"

namespace qps {
auto IfEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<Product> {
  std::vector<Product> if_products;
  Ref ref1 = getClause().getArg1();

  if (std::holds_alternative<QuotedIdentifier>(ref1)) {
    auto
        *ifStatementContainingPattern = pkb.getLeftEntitiesRelatedToGivenRightEntity(
            UsesInParentConditionRelationship::getRelationshipTypeStatic(), IfStatement::getEntityTypeStatic(),
            Variable::getEntityTypeStatic(), std::get<QuotedIdentifier>(ref1).getQuotedId());
    for (const auto &row : *ifStatementContainingPattern) {
      auto product = Product(row);
      if_products.push_back(product);
    }
  } else {
    auto *all_if_variable_pairs =
        pkb.getRelationshipsByTypes(UsesInParentConditionRelationship::getRelationshipTypeStatic(),
                                    IfStatement::getEntityTypeStatic(),
                                    Variable::getEntityTypeStatic());
    for (const auto &row : *all_if_variable_pairs) {
      auto product = Product(row);
      if_products.push_back(product);
    }
  }
  return if_products;
}
}  // namespace qps
