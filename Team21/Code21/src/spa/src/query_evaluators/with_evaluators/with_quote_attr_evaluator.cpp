#include "with_quote_attr_evaluator.h"
#include <unordered_set>
#include <vector>

namespace qps {
auto WithQuoteAttrEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> equalResult = {};
  std::vector<Entity *> firstResult = {};

  std::vector<Declaration> decl = getDeclarations();

  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();

  AttrRef atrVal = std::get<AttrRef>(ref1.ref);
  QuotedIdentifier quoteVal = std::get<QuotedIdentifier>(ref2.ref);

  // Swap if ref1 is not an AttrRef since it will be an QuotedIdentifier, vice versa
  if (std::get_if<int>(&ref1.ref) != nullptr) {
    atrVal = std::get<AttrRef>(ref2.ref);
    quoteVal = std::get<QuotedIdentifier>(ref1.ref);
  }

  auto declAtrVal = Declaration::findDeclarationWithSynonym(decl, atrVal.synonym);

  EntityType entType = ClauseEvaluator::DesignEntityToEntityType(declAtrVal.value().getDesignEntity());

  auto *pkb_res = pkb.getEntity(entType, quoteVal.getQuotedId());

  if (pkb_res != nullptr) {
    firstResult.push_back(pkb_res);
    equalResult.push_back(firstResult);
    return equalResult;
  }

  return equalResult;
}
}  // namespace qps
