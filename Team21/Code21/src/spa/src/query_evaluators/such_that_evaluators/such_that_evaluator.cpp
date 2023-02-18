#include "such_that_evaluator.h"
#include "query/ref.h"
#include "query/query_exceptions.h"

namespace qps {
const std::vector<EntityType>
    SuchThatEvaluator::CONTAINER_TYPES = {EntityType::WHILE_STATEMENT, EntityType::IF_STATEMENT};

SuchThatEvaluator::SuchThatEvaluator(SuchThatClause clause, std::vector<Declaration> declarations)
    : clause_(std::move(clause)), declarations_(std::move(declarations)) {}

ClauseEvaluator::ClauseResult SuchThatEvaluator::Evaluate(QueryFacade &pkb) {
  std::vector<::Relationship *> filtered_relationships;
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  auto left_hand = ResolveLeftTypes(ref1);
  auto right_hand = ResolveRightTypes(ref2);
  for (auto left : left_hand) {
    for (auto right : right_hand) {
      auto res = CallPkb(pkb, left, right);
      for (auto relationship : res) {
        if (!Filter(*relationship)) {
          filtered_relationships.push_back(relationship);
        }
      }
    }
  }
  return qps::SuchThatEvaluator::ConstructResult(filtered_relationships);
}

ClauseEvaluator::ClauseResult SuchThatEvaluator::ConstructResult(const std::vector<::Relationship *> &relationships) {
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  std::vector<Synonym> syns;
  bool left_syn = false;
  bool right_syn = false;
  if (Synonym *syn = std::get_if<Synonym>(&ref1)) {
    left_syn = true;
    syns.push_back(*syn);
  }
  if (Synonym *syn = std::get_if<Synonym>(&ref2)) {
    right_syn = true;
    syns.push_back(*syn);
  }

  if (!left_syn && !right_syn) return !relationships.empty();

  SynonymTable result(syns);
  for (auto relation : relationships) {
    SynonymTable::Row row;
    if (left_syn) row.push_back(*relation->getLeftHandEntity()->getEntityValue());
    if (right_syn) row.push_back(*relation->getRightHandEntity()->getEntityValue());
    result.AddRow(row);
  }
  return result;
}

// TODO: refactor.
// This function has a lot of repetition.
bool SuchThatEvaluator::Filter(::Relationship &relationship) {
  Ref ref1 = clause_.getArg1();
  Ref ref2 = clause_.getArg2();
  if (StatementNumber *stmt_num = std::get_if<StatementNumber>(&ref1)) {
    if (*(relationship.getLeftHandEntity()->getEntityValue()) != std::to_string(*stmt_num)) {
      return true;
    }
  }

  if (StatementNumber *stmt_num = std::get_if<StatementNumber>(&ref2)) {
    if (*(relationship.getRightHandEntity()->getEntityValue()) != std::to_string(*stmt_num)) {
      return true;
    }
  }

  if (QuotedIdentifier *quoted_identifier = std::get_if<QuotedIdentifier>(&ref1)) {
    if (*(relationship.getLeftHandEntity()->getEntityValue()) != quoted_identifier->getQuotedId()) {
      return true;
    }
  }

  if (QuotedIdentifier *quoted_identifier = std::get_if<QuotedIdentifier>(&ref2)) {
    if (*(relationship.getRightHandEntity()->getEntityValue()) != quoted_identifier->getQuotedId()) {
      return true;
    }
  }
  return false;
}

EntityType SuchThatEvaluator::FindEntityType(Synonym &syn) {
  std::optional<Declaration> decl = Declaration::findDeclarationWithSynonym(declarations_, syn);
  if (decl) {
    return ClauseEvaluator::DesignEntityToEntityType(decl.value().getDesignEntity());
  } else {
    throw std::invalid_argument("Synonym in clause not in query declaration");
  }
}
std::vector<EntityType> SuchThatEvaluator::ResolveLeftTypes(Ref &left_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&left_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return GetLeftHandTypes(left_arg);
  }
}

std::vector<EntityType> SuchThatEvaluator::ResolveRightTypes(Ref &right_arg) {
  if (Synonym *syn = std::get_if<Synonym>(&right_arg)) {
    return {FindEntityType(*syn)};
  } else {
    return GetRightHandTypes(right_arg);
  }
}
} // qps
