#include "with_quote_attr_evaluator.h"
#include <vector>

#include "query_evaluators/pkb_qps_type_mapping.h"

namespace qps {
auto WithQuoteAttrEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> equalResult = {};
  std::vector<Entity *> firstResult = {};

  std::vector<Declaration> decl = getDeclarations();

  const auto &ref1 = getClause().getRef1();
  const auto &ref2 = getClause().getRef2();

  auto *atrVal = std::get_if<AttrRef>(&ref1.ref);
  auto *quoteVal = std::get_if<QuotedIdentifier>(&ref2.ref);

  // Swap if ref1 is not an AttrRef since it will be an QuotedIdentifier, vice versa
  if (atrVal == nullptr) {
    atrVal = std::get_if<AttrRef>(&ref2.ref);
    quoteVal = std::get_if<QuotedIdentifier>(&ref1.ref);
  }

  auto declAtrVal = Declaration::findDeclarationWithSynonym(decl, atrVal->synonym);

  const auto &entType = DesignEntityToEntityType(declAtrVal.value().getDesignEntity());

  auto *entities = pkb.getEntitiesByType(entType);
  for (auto &entity : *entities) {
    if (AttributeReferenceEvaluator::EvaluateAttrRef(entity, atrVal->attrName, pkb) == quoteVal->getQuotedId()) {
      firstResult.push_back(entity);
    }
  }

  if (!firstResult.empty()) {
    equalResult.push_back(firstResult);
  }

  return equalResult;
}
}  // namespace qps
