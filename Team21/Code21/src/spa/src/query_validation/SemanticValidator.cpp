#include <unordered_set>
#include <string>
#include <variant>

#include "SemanticValidator.h"

namespace qps {

SemanticValidator::SemanticValidator(Query query) : QueryValidator(query) {}

bool SemanticValidator::validateQuery() {
  checkForDuplicateDeclarations();
  checkIfSynonymContainedInDeclaration();
  checkPatternClauseSynAssign();
  checkNoWildCardFirstArgModifiesUses();
  checkRelationSynonymMatchDesignEntity();
  checkPatternSynonymMatchDesignEntity();
  return true;
}

void SemanticValidator::checkForDuplicateDeclarations() {
  //create a hashset and then add to it. If current iterable element is found in hashset, it means duplicate.
  std::unordered_set<std::string> seen;
  for (auto &declr : getQuery().getDeclarations()) {
    std::string syn = declr.getSynonym().getSynonym();
    if (seen.find(syn) == seen.end()) {
      seen.insert(syn);
    } else {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is duplicate declaration found for " + syn);
    }
  }
}

void SemanticValidator::checkIfSynonymContainedInDeclaration() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  Synonym syn = getQuery().getSelectClause().getSynonym();
  std::vector<SuchThatClause> s = getQuery().getSuchThatClause();
  std::vector<PatternClause> p = getQuery().getPatternClause();
  if (Declaration::findDeclarationWithSynonym(declr, syn).has_value() == false) {
    throw QueryException(ErrorType::Semantic,
                         "Semantic error. There is missing declaration in Select clause for " + syn.getSynonym());
  }
  for (int i = 0; i < s.size(); i++) {
    Ref refSuchThat1 = s[i].getArg1();
    Ref refSuchThat2 = s[i].getArg2();
    checkSynonymDeclareHelper(refSuchThat1, declr, "SuchThat clause for argument 1");
    checkSynonymDeclareHelper(refSuchThat2, declr, "SuchThat clause for argument 2");
  }
  for (int i = 0; i < p.size(); i++) {
    Ref refPattern = p[i].getArg1();
    checkSynonymDeclareHelper(refPattern, declr, "AssignPattern clause for argument 1");
  }
}

void SemanticValidator::checkSynonymDeclareHelper(Ref r, std::vector<Declaration> declr, std::string missing) {
  if (std::holds_alternative<Synonym>(r)) {
    if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(r)).has_value() == false) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in " + missing);
    }
  }
}

void SemanticValidator::checkNoWildCardFirstArgModifiesUses() {
  std::vector<SuchThatClause> s = getQuery().getSuchThatClause();
  for (int i = 0; i < s.size(); i++) {
    Relationship rel = s[i].getRelationship();
    if (std::holds_alternative<Underscore>(s[i].getArg1())) {
      if (rel == Relationship::ModifiesP || rel == Relationship::ModifiesS || rel == Relationship::UsesP
          || rel == Relationship::UsesS || rel == Relationship::Modifies || rel == Relationship::Uses) {
        std::string relStr = getStringFromRelationship(rel);
        throw QueryException(ErrorType::Semantic, "Semantic error. There is wild card as first argument in " + relStr);
      }
    }
  }
}

void SemanticValidator::checkRelationSynonymMatchDesignEntity() {
  std::vector<SuchThatClause> s = getQuery().getSuchThatClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (int i = 0; i < s.size(); i++) {
    Relationship rel = s[i].getRelationship();
    std::string relStr = getStringFromRelationship(rel);
    if (std::holds_alternative<Synonym>(s[i].getArg1())) {
      Synonym syn = std::get<Synonym>(s[i].getArg1());
      DesignEntity d = Declaration::findDeclarationWithSynonym(declr, syn).value().getDesignEntity();
      if ((rel == Relationship::ModifiesP || rel == Relationship::UsesP || rel == Relationship::Calls
          || rel == Relationship::CallsT)) {
        if (d != DesignEntity::PROCEDURE) {
          throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
        }
      } else {
        checkSynonymStatementHelper(d, relStr);
      }
    }
    if (std::holds_alternative<Synonym>(s[i].getArg2())) {
      Synonym syn2 = std::get<Synonym>(s[i].getArg2());
      DesignEntity d2 = Declaration::findDeclarationWithSynonym(declr, syn2).value().getDesignEntity();
      if (rel == Relationship::Calls || rel == Relationship::CallsT) {
        if (d2 != DesignEntity::PROCEDURE) {
          throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
        }
      } else if ((rel == Relationship::ModifiesP || rel == Relationship::UsesP || rel == Relationship::ModifiesS
          || rel == Relationship::UsesS)) {
        if (d2 != DesignEntity::VARIABLE) {
          throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
        }
      } else {
        checkSynonymStatementHelper(d2, relStr);
      }
    }
  }
}

void SemanticValidator::checkSynonymStatementHelper(DesignEntity d, std::string relStr) {
  if (d != DesignEntity::STMT && d != DesignEntity::READ && d != DesignEntity::PRINT && d != DesignEntity::ASSIGN
      && d != DesignEntity::IF && d != DesignEntity::WHILE && d != DesignEntity::CALL) {
    throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
  }
}

void SemanticValidator::checkPatternSynonymMatchDesignEntity() {
  std::vector<PatternClause> p = getQuery().getPatternClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (int i = 0; i < p.size(); i++) {
      if (std::holds_alternative<Synonym>(p[i].getArg1())) {
          Synonym syn = std::get<Synonym>(p[i].getArg1());
          DesignEntity patternArgSyn = Declaration::findDeclarationWithSynonym(declr, syn).value().getDesignEntity();
          if (patternArgSyn != DesignEntity::VARIABLE) {
              throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for pattern argument 1");
          }
      }
  }
}
}
