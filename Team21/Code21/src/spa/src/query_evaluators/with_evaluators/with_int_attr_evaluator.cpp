#include "with_int_attr_evaluator.h"
#include <vector>

#include "query_evaluators/pkb_helpers.h"

namespace qps {
auto WithIntAttrEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> equalResult = {};
  std::vector<Entity *> firstResult = {};

  std::vector<Declaration> decl = getDeclarations();

  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();

  AttrRef atrVal = std::get<AttrRef>(ref1.ref);
  int numberVal = std::get<int>(ref2.ref);

  // Swap if ref1 is not an AttrRef since it will be an int, vice versa
  if (std::get_if<int>(&ref1.ref) != nullptr) {
    atrVal = std::get<AttrRef>(ref2.ref);
    numberVal = std::get<int>(ref1.ref);
  }

  auto declAtrVal = Declaration::findDeclarationWithSynonym(decl, atrVal.synonym);

  EntityType entType = DesignEntityToEntityType(declAtrVal.value().getDesignEntity());

  auto *pkb_res = pkb.getEntity(entType, numberVal);

  if (pkb_res != nullptr) {
    firstResult.push_back(pkb_res);
    equalResult.push_back(firstResult);
    return equalResult;
  }

  return equalResult;
}
}  // namespace qps