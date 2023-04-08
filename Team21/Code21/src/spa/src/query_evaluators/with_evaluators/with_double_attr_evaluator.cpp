#include "with_double_attr_evaluator.h"
#include <vector>

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "query/attr_ref.h"
#include "query/declaration.h"
#include "query_evaluators/clause_evaluator.h"
#include "query_evaluators/pkb_helpers.h"

namespace qps {
auto WithDoubleAttrEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> equalResult = {};
  std::vector<Entity *> firstResult = {};
  std::vector<Entity *> secondResult = {};

  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();
  std::vector<Declaration> decl = getDeclarations();

  AttrRef ref1Attr = std::get<AttrRef>(ref1.ref);
  AttrRef ref2Attr = std::get<AttrRef>(ref2.ref);

  auto decl1 = Declaration::findDeclarationWithSynonym(decl, ref1Attr.synonym);
  auto decl2 = Declaration::findDeclarationWithSynonym(decl, ref2Attr.synonym);

  const EntityType &ent1 = DesignEntityToEntityType(decl1.value().getDesignEntity());
  const EntityType &ent2 = DesignEntityToEntityType(decl2.value().getDesignEntity());

  auto *pkb_res1 = pkb.getEntitiesByType(ent1);
  auto *pkb_res2 = pkb.getEntitiesByType(ent2);

  for (const auto &res1 : *pkb_res1) {
    for (const auto &res2 : *pkb_res2) {
      auto attrValue1 = AttributeReferenceEvaluator::EvaluateAttrRef(res1, ref1Attr.attrName, pkb);
      auto attrValue2 = AttributeReferenceEvaluator::EvaluateAttrRef(res2, ref2Attr.attrName, pkb);
      if (attrValue1 == attrValue2) {
        firstResult.push_back(res1);
        secondResult.push_back(res2);
      }
    }
  }

  equalResult.push_back(firstResult);
  equalResult.push_back(secondResult);
  return equalResult;
}
}  // namespace qps
