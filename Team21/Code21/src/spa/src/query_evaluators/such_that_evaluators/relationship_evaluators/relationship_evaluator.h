#pragma once

#include "query/declaration.h"
#include "query/such_that_clause.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

class RelationshipEvaluator : public SuchThatEvaluator {
 public:
  static const std::vector<EntityType> CONTAINER_TYPES;

  RelationshipEvaluator(SuchThatClause clause, std::vector<Declaration> declarations) :
      SuchThatEvaluator(std::move(clause), std::move(declarations)) {}

  auto Evaluate(QueryFacade &pkb) -> ClauseResult override;

  virtual auto CallPkb(QueryFacade &pkb, EntityType left, EntityType right) -> std::vector<::Relationship *> = 0;
  virtual auto GetLeftHandTypes(Ref &left_arg) -> std::vector<EntityType> = 0;
  virtual auto GetRightHandTypes(Ref &right_arg) -> std::vector<EntityType> = 0;

 private:
  auto Filter(::Relationship &relationship) -> bool;
  auto ResolveLeftTypes(Ref &left_arg) -> std::vector<EntityType>;
  auto ResolveRightTypes(Ref &right_arg) -> std::vector<EntityType>;
  auto ConstructResult(const std::vector<::Relationship *> &) -> ClauseResult;
};

}  // namespace qps
