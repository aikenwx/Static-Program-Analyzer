#pragma once

#include "query/declaration.h"
#include "query/ref.h"
#include "query/relationship.h"

#include <variant>
#include <vector>

namespace qps {

// SuchThatClause represents relRef type in Such That Clause
class SuchThatClause {
private:
  Relationship relationship;
  Ref arg1;
  Ref arg2;

  //Helper function to deal with special relationship: Modifies and Uses are split into 2 types S and P
  void handleModifiesUses(std::vector<Declaration> &declarations) {
    if (relationship != Relationship::Uses &&
        relationship != Relationship::Modifies) {
      return;
    }
    // Handle Synonym case
    if (std::holds_alternative<Synonym>(arg1)) {
      auto *synonym = std::get_if<Synonym>(&arg1);
      auto declaration =
          Declaration::findDeclarationWithSynonym(declarations, *synonym);
      if (declaration->getDesignEntity() == DesignEntity::PROCEDURE) {
        relationship = (relationship == Relationship::Uses)
                           ? Relationship::UsesP
                           : Relationship::ModifiesP;
      } else {
        relationship = (relationship == Relationship::Uses)
                           ? Relationship::UsesS
                           : Relationship::ModifiesS;
      }
    // Handle Quoted Identifier case
    } else if (std::holds_alternative<QuotedIdentifier>(arg1)) {
      relationship = (relationship == Relationship::Uses)
                         ? Relationship::UsesP
                         : Relationship::ModifiesP;
    // Handle Statement Number case
    } else if (std::holds_alternative<StatementNumber>(arg1)) {
      relationship = (relationship == Relationship::Uses)
                         ? Relationship::UsesS
                         : Relationship::ModifiesS;
    }
  }

public:
  auto getRelationship() -> Relationship;
  auto getArg1() -> Ref;
  auto getArg2() -> Ref;
  SuchThatClause(Relationship relationship_, Ref arg1_, Ref arg2_,
                 std::vector<Declaration> &declarations);

  auto operator==(const SuchThatClause &clause) const -> bool {
    return relationship == clause.relationship && arg1 == clause.arg1 &&
           arg2 == clause.arg2;
  }
  friend auto operator<<(std::ostream &osstream, SuchThatClause const &clause)
      -> std::ostream & {
    osstream << getStringFromRelationship(clause.relationship);
    return osstream;
  }
};
} // namespace qps
