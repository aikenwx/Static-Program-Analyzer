#include <unordered_set>
#include <string>
#include <variant>

#include "SemanticValidator.h"

namespace qps {

SemanticValidator::SemanticValidator(Query query) : QueryValidator(query) {}

bool SemanticValidator::validateQuery() {
  checkForDuplicateDeclarations();
  checkIfSynonymContainedInDeclaration();
  checkPatternClauseSynonym();
  checkNoWildCardFirstArgModifiesUses();
  checkRelationSynonymMatchDesignEntity();
  checkPatternSynonymMatchDesignEntity();
  checkWithClauseSameAttributeCompare();
  checkAttrRefValidAttrName();
  return true;
}

void SemanticValidator::checkForDuplicateDeclarations() {
  //create a hashset and then add to it. If current iterable element is found in hashset, it means duplicate.
  std::unordered_set<std::string> seen;
  for (Declaration declr : getQuery().getDeclarations()) {
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
  Result result = getQuery().getSelectClause();
  if (std::holds_alternative<std::vector<Element>>(result)) {
    std::vector<Element> tuple = std::get<std::vector<Element>>(result);
    for (int i = 0; i < tuple.size(); i++) {
      checkElementSynonymDeclareHelper(tuple[i], declr);
    }
  }
  std::vector<SuchThatClause> s = getQuery().getSuchThatClause();
  std::vector<PatternClause> p = getQuery().getPatternClause();
  std::vector<WithClause> w = getQuery().getWithClause();
  for (int i = 0; i < s.size(); i++) {
    Ref refSuchThat1 = s[i].getArg1();
    Ref refSuchThat2 = s[i].getArg2();
    checkSynonymDeclareHelper(refSuchThat1, declr, "SuchThat clause for argument 1");
    checkSynonymDeclareHelper(refSuchThat2, declr, "SuchThat clause for argument 2");
  }
  for (int i = 0; i < p.size(); i++) {
    Synonym stmtSyn = p[i].getStmtSynonym();
    if (Declaration::findDeclarationWithSynonym(declr, stmtSyn).has_value() == false) {
      throw QueryException(ErrorType::Semantic,
        "Semantic error. There is missing declaration in Select clause for " + stmtSyn.getSynonym());
    }
    Ref refPattern = p[i].getArg1();
    checkSynonymDeclareHelper(refPattern, declr, "Pattern clause for argument 1");
  }
  for (int i = 0; i < w.size(); i++) {
    WithRef ref1 = w[i].getRef1();
    WithRef ref2 = w[i].getRef2();
    checkWithRefSynonymDeclareHelper(ref1, declr);
    checkWithRefSynonymDeclareHelper(ref2, declr);
  }
}

void SemanticValidator::checkPatternClauseSynonym() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  if (!getQuery().getPatternClause().empty()) {
    Synonym syn = getQuery().getPatternClause()[0].getStmtSynonym();
    std::optional<Declaration> patternDclr = Declaration::findDeclarationWithSynonym(declr, syn);
    if (patternDclr.has_value()) {
      if (patternDclr.value().getDesignEntity() != DesignEntity::ASSIGN
        && patternDclr.value().getDesignEntity() != DesignEntity::WHILE
        && patternDclr.value().getDesignEntity() != DesignEntity::IF) {
        throw QueryException(ErrorType::Semantic,
          "Semantic error. Invalid syntax for pattern with synonym: " + syn.getSynonym());
      }
    }
  }
}

void SemanticValidator::checkSynonymDeclareHelper(Ref r, std::vector<Declaration>& declr, std::string missing) {
  if (std::holds_alternative<Synonym>(r)) {
    if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(r)).has_value() == false) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in " + missing);
    }
  }
}

void SemanticValidator::checkElementSynonymDeclareHelper(Element r, std::vector<Declaration>& declr) {
  if (std::holds_alternative<Synonym>(r)) {
    if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(r)).has_value() == false) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration for synonym " +
        std::get<Synonym>(r).getSynonym());
    }
  }
  else if (std::holds_alternative<AttrRef>(r)) {
    if (Declaration::findDeclarationWithSynonym(declr, std::get<AttrRef>(r).synonym).has_value() == false) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration for synonym in AttrRef " +
        std::get<AttrRef>(r).synonym.getSynonym());
    }
  }
}

void SemanticValidator::checkWithRefSynonymDeclareHelper(WithRef r, std::vector<Declaration>& declr) {
  if (std::holds_alternative<AttrRef>(r.ref)) {
    if (Declaration::findDeclarationWithSynonym(declr, std::get<AttrRef>(r.ref).synonym).has_value() == false) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration for synonym in WithRef" +
        std::get<AttrRef>(r.ref).synonym.getSynonym());
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

void SemanticValidator::checkWithClauseSameAttributeCompare() {
  std::vector<WithClause> w = getQuery().getWithClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (int i = 0; i < w.size(); i++) {
    if (w[i].getRef1().attrType != w[i].getRef2().attrType) {
      throw QueryException(ErrorType::Semantic, "Comparison of different type of attribute in with clause");
    }
  }
}

void SemanticValidator::checkAttrRefValidAttrName() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  Result result = getQuery().getSelectClause();
  if (std::holds_alternative<std::vector<Element>>(result)) {
    std::vector<Element> tuple = std::get<std::vector<Element>>(result);
    for (int i = 0; i < tuple.size(); i++) {
      if (std::holds_alternative<AttrRef>(tuple[i])) {
        checkAttrRefValidAttrNameHelper(std::get<AttrRef>(tuple[i]), declr);
      }
    }
  }
  std::vector<WithClause> w = getQuery().getWithClause();
  for (int i = 0; i < w.size(); i++) {
    WithRef ref1 = w[i].getRef1();
    WithRef ref2 = w[i].getRef2();
    checkAttrRefValidAttrNameHelper(std::get<AttrRef>(ref1.ref), declr);
    checkAttrRefValidAttrNameHelper(std::get<AttrRef>(ref2.ref), declr);
  }
}

void SemanticValidator::checkAttrRefValidAttrNameHelper(AttrRef ar, std::vector<Declaration>& declr) {
  if (Declaration::findDeclarationWithSynonym(declr, ar.synonym).has_value()) {
    Declaration decl = Declaration::findDeclarationWithSynonym(declr, ar.synonym).value();
    std::set<AttrName> validAttrNameSet = getValidAttrNameSet(decl);
    if (validAttrNameSet.find(ar.attrName) == validAttrNameSet.end()) {
      throw QueryException(ErrorType::Semantic, "Attribute name is invalid with synonym " + ar.synonym.getSynonym());
    }
  }
}

}
