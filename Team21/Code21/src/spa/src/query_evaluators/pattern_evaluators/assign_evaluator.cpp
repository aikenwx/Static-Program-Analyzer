#include "assign_evaluator.h"
#include "query/query_exceptions.h"

namespace qps {
std::vector<Relationship *> AssignEvaluator::CallPkb(QueryFacade &pkb) {
  std::vector<ModifiesRelationship *> modifies_relationships;
  Ref ref1 = clause_.getArg1();

  auto all_assignment_variable_pairs =
      pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(AssignStatement::getEntityTypeStatic(), Variable::getEntityTypeStatic());
  for (const auto &row : *all_assignment_variable_pairs) {
    if (std::holds_alternative<QuotedIdentifier>(ref1)
        && *row->getRightHandEntity()->getEntityValue() != std::get<QuotedIdentifier>(ref1).getQuotedId()) {
      continue;
    } else {
      modifies_relationships.push_back(row);
    }
  }

  ExpressionSpec expr = clause_.getArg2();
  if (std::holds_alternative<Expression>(expr)) {
    Expression express = std::get<Expression>(expr);
    std::string expr = express.getExpression();
    bool is_partial = express.isExpressionPartial();
    modifies_relationships = checkExpressionContained(modifies_relationships, expr, is_partial);
  }

  std::vector<Relationship *> res;
  res.assign(modifies_relationships.begin(), modifies_relationships.end());
  return res;
}

std::vector<ModifiesRelationship *> AssignEvaluator::checkExpressionContained(std::vector<ModifiesRelationship *> assignment_modifies,
                                                                              std::string postfix,
                                                                              bool is_partial) {
  std::vector<ModifiesRelationship *> filtered_relationships;
  for (auto relationship : assignment_modifies) {
    auto assignment_stmt = dynamic_cast<AssignStatement *>(relationship->getLeftHandEntity());
    auto post_fix_espression = *assignment_stmt->getPostFixExpression();
    bool expression_contained =
        (is_partial && post_fix_espression.find(postfix) != std::string::npos) || (postfix == post_fix_espression);
    if (expression_contained) {
      filtered_relationships.push_back(relationship);
    }
  }
  return filtered_relationships;

}
} // qps
