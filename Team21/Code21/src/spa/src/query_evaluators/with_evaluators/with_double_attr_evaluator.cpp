#include "with_double_attr_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "query/attr_ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/with_evaluators/with_double_attr_evaluator.h"
#include <unordered_set>
#include <vector>

namespace qps {
auto WithDoubleAttrEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>*> {
  std::vector<std::vector<Entity *>*> equalResult;
  std::vector<Entity *> trueResult;
  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();

  AttrRef ref1Attr = std::get<AttrRef>(ref1.ref);
  AttrRef ref2Attr = std::get<AttrRef>(ref2.ref);

  equalResult.push_back(&trueResult);
  return equalResult;
  // std::optional<Declaration> decl = Declaration::findDeclarationWithSynonym(declarations_, syn);
  // if (decl) {
  //   return ClauseEvaluator::DesignEntityToEntityType(decl.value().getDesignEntity());
  // } else {


  // Ref ref1 = getClause().getArg1();

  // if (std::holds_alternative<QuotedIdentifier>(ref1)) {
  //   std::unordered_set<WhileStatement *>
  //       *set = pkb.getWhileStatementsUsingVariableInCondition(std::get<QuotedIdentifier>(ref1).getQuotedId());
  //   for (const auto &row : *set) {
  //     auto product = Product(row);
  //     while_products.push_back(product);
  //   }
  // } else {
  //   auto *all_while_variable_pairs =
  //       pkb.getUsesRelationshipsByLeftAndRightEntityTypes(WhileStatement::getEntityTypeStatic(),
  //                                                         Variable::getEntityTypeStatic());
  //   for (const auto &row : *all_while_variable_pairs) {
  //     auto product = Product(row);
  //     while_products.push_back(product);
  //   }
  // }
  return equalResult;
}
}  // namespace qps
