#pragma once

#include "query/declaration.h"
#include "query/such_that_clause.h"
#include "query_evaluators/clause_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

class SuchThatEvaluator : public ClauseEvaluator {
 public:
  static const std::vector<EntityType> CONTAINER_TYPES;

  SuchThatEvaluator(SuchThatClause clause, std::vector<Declaration> declarations);

  EntityType FindEntityType(Synonym &syn);

  ClauseResult Evaluate(QueryFacade &pkb) override;

  virtual std::vector<::Relationship *> CallPkb(QueryFacade &pkb, EntityType left, EntityType right) = 0;
  virtual std::vector<EntityType> GetLeftHandTypes(Ref &left_arg) = 0;
  virtual std::vector<EntityType> GetRightHandTypes(Ref &right_arg) = 0;

 private:
  bool Filter(::Relationship &relationship);

  ClauseResult ConstructResult(const std::vector<::Relationship *> &);
  SuchThatClause clause_;
  std::vector<Declaration> declarations_;

};

} // qps
