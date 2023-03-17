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

  ClauseResult Evaluate(QueryFacade &pkb) override;

  virtual std::vector<::Relationship *> CallPkb(QueryFacade &pkb, EntityType left, EntityType right) = 0;
  virtual std::vector<EntityType> GetLeftHandTypes(Ref &left_arg) = 0;
  virtual std::vector<EntityType> GetRightHandTypes(Ref &right_arg) = 0;

 private:
  bool Filter(::Relationship &relationship);
  std::vector<EntityType> ResolveLeftTypes(Ref &left_arg);
  std::vector<EntityType> ResolveRightTypes(Ref &right_arg);
  ClauseResult ConstructResult(const std::vector<::Relationship *> &);
};

} // qps
