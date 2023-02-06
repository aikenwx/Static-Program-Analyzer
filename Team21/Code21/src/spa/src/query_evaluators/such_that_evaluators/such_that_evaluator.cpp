#include "such_that_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {
const std::vector<EntityType>
    SuchThatEvaluator::CONTAINER_TYPES = {EntityType::WHILE_STATEMENT, EntityType::IF_STATEMENT};

SuchThatEvaluator::SuchThatEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

ClauseResult SuchThatEvaluator::evaluate(QueryFacade &pkb) {
  std::vector<::Relationship *> filtered_relationships;
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  auto left_hand = GetLeftHandTypes(ref1);
  auto right_hand = GetRightHandTypes(ref2);
  for (auto left : left_hand) {
    for (auto right : right_hand) {
      auto res = callPKB(pkb, left, right);
      for (auto relationship : res) {
        if (!filter(*relationship)) {
          filtered_relationships.push_back(relationship);
        }
      }
    }
  }
  return qps::SuchThatEvaluator::ConstructResult(filtered_relationships);
}

ClauseResult SuchThatEvaluator::ConstructResult(const std::vector<::Relationship *> &relationships) {
  ClauseResult result;
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  if (Synonym *syn = std::get_if<Synonym>(&ref1)) {
    result.AddSynonym(*syn, 0);
  }

  if (Synonym *syn = std::get_if<Synonym>(&ref2)) {
    result.AddSynonym(*syn, 1);
  }

  for (auto relation : relationships) {
    auto res = {relation->getLeftHandEntity()->getEntityValue(), relation->getRightHandEntity()->getEntityValue()};
    result.AddResult({res});
  }
  return result;
}

// TODO: refactor.
// This function has a lot of repetition.
bool SuchThatEvaluator::filter(::Relationship &r) {
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  if (StatementNumber *stmt_num = std::get_if<StatementNumber>(&ref1)) {
    if (r.getLeftHandEntity()->getEntityValue() != std::to_string(*stmt_num)) {
      return true;
    }
  }

  if (StatementNumber *stmt_num = std::get_if<StatementNumber>(&ref2)) {
    if (r.getRightHandEntity()->getEntityValue() != std::to_string(*stmt_num)) {
      return true;
    }
  }

  if (QuotedIdentifier *quoted_identifier = std::get_if<QuotedIdentifier>(&ref1)) {
    if (r.getLeftHandEntity()->getEntityValue() != quoted_identifier->getQuotedId()) {
      return true;
    }
  }

  if (QuotedIdentifier *quoted_identifier = std::get_if<QuotedIdentifier>(&ref2)) {
    if (r.getRightHandEntity()->getEntityValue() != quoted_identifier->getQuotedId()) {
      return true;
    }
  }
  return false;
}

EntityType SuchThatEvaluator::FindEntityType(Synonym &syn) {
  std::optional<Declaration> decl = Declaration::findDeclarationWithSynonym(declarations_, syn);
  if (decl) {
    return ClauseEvaluator::design_entity_to_entity_type(decl.value().getDesignEntity());
  } else {
    throw QueryException("Synonym in clause not in query declaration");
  }
}
} // qps
