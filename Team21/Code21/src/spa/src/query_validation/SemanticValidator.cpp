#include <unordered_set>
#include <string>
#include <variant>

#include "SemanticValidator.h"

namespace qps {

SemanticValidator::SemanticValidator(Query& query) : QueryValidator(query) {}

//checks semantically query by doing checks
auto SemanticValidator::validateQuery() -> bool {
  checkForDuplicateDeclarations();
  checkIfSynonymContainedInDeclaration();
  checkPatternClauseSynonym();
  checkNoWildCardFirstArgModifiesUses();
  checkRelationSynonymMatchDesignEntity();
  checkPatternSynonymMatchDesignEntity();
  checkAttrRefValidAttrName();
  checkWithClauseSameAttributeCompare();
  return true;
}

//checks for no duplicate declarations
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

//checks that all synonyms in clauses are from declarations
void SemanticValidator::checkIfSynonymContainedInDeclaration() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  Result result = getQuery().getSelectClause();
  if (std::holds_alternative<std::vector<Element>>(result)) {
    std::vector<Element> tuple = std::get<std::vector<Element>>(result);
    for (auto & ele : tuple) {
      checkElementSynonymDeclareHelper(ele, declr);
    }
  }
  std::vector<SuchThatClause> suchClauses = getQuery().getSuchThatClause();
  std::vector<PatternClause> patternClauses = getQuery().getPatternClause();
  std::vector<WithClause> withClauses = getQuery().getWithClause();
  for (auto & suchClause : suchClauses) {
    Ref refSuchThat1 = suchClause.getArg1();
    Ref refSuchThat2 = suchClause.getArg2();
    checkSynonymDeclareHelper(refSuchThat1, declr, "SuchThat clause for argument 1");
    checkSynonymDeclareHelper(refSuchThat2, declr, "SuchThat clause for argument 2");
  }
  for (auto & patternClause : patternClauses) {
    Ref refPattern = patternClause.getArg1();
    checkSynonymDeclareHelper(refPattern, declr, "Pattern clause for argument 1");
  }
  for (auto & withClause : withClauses) {
    WithRef ref1 = withClause.getRef1();
    WithRef ref2 = withClause.getRef2();
    checkWithRefSynonymDeclareHelper(ref1, declr);
    checkWithRefSynonymDeclareHelper(ref2, declr);
  }
}

//check that the pattern clause has a synonym with valid design entity
void SemanticValidator::checkPatternClauseSynonym() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  std::vector<PatternClause> patternClauses = getQuery().getPatternClause();
  for (auto & patternClause : patternClauses) {
    Synonym syn = patternClause.getStmtSynonym();
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

//checks that no wild card as first argument for modifies and uses
void SemanticValidator::checkNoWildCardFirstArgModifiesUses() {
  std::vector<SuchThatClause> suchClauses = getQuery().getSuchThatClause();
  for (auto & suchClause : suchClauses) {
    Relationship rel = suchClause.getRelationship();
    if (std::holds_alternative<Underscore>(suchClause.getArg1())) {
      if (rel == Relationship::ModifiesP || rel == Relationship::ModifiesS || rel == Relationship::UsesP
          || rel == Relationship::UsesS || rel == Relationship::Modifies || rel == Relationship::Uses) {
        std::string relStr = getStringFromRelationship(rel);
        throw QueryException(ErrorType::Semantic, "Semantic error. There is wild card as first argument in " + relStr);
      }
    }
  }
}

//checks that synonyms declared in such that clause for relationships(e.g. parent) have the correct design entity types for that relation
void SemanticValidator::checkRelationSynonymMatchDesignEntity() {
  std::vector<SuchThatClause> suchClauses = getQuery().getSuchThatClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (auto & suchClause : suchClauses) {
    Relationship rel = suchClause.getRelationship();
    std::string relStr = getStringFromRelationship(rel);
    if (std::holds_alternative<Synonym>(suchClause.getArg1())) {
      Synonym syn = std::get<Synonym>(suchClause.getArg1());
      DesignEntity des = Declaration::findDeclarationWithSynonym(declr, syn).value().getDesignEntity();
      if ((rel == Relationship::ModifiesP || rel == Relationship::UsesP || rel == Relationship::Calls
          || rel == Relationship::CallsT)) {
        if (des != DesignEntity::PROCEDURE) {
          throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
        }
      } else {
        checkSynonymStatementHelper(des, relStr);
      }
    }
    if (std::holds_alternative<Synonym>(suchClause.getArg2())) {
      Synonym syn2 = std::get<Synonym>(suchClause.getArg2());
      DesignEntity des2 = Declaration::findDeclarationWithSynonym(declr, syn2).value().getDesignEntity();
      checkSecondSynonymHelper(des2, relStr, rel);
    }
  }
}

//checks that pattern first arg is variable if it is synonym
void SemanticValidator::checkPatternSynonymMatchDesignEntity() {
  std::vector<PatternClause> patternClauses = getQuery().getPatternClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (auto & clau : patternClauses) {
      if (std::holds_alternative<Synonym>(clau.getArg1())) {
          Synonym syn = std::get<Synonym>(clau.getArg1());
          DesignEntity patternArgSyn = Declaration::findDeclarationWithSynonym(declr, syn).value().getDesignEntity();
          if (patternArgSyn != DesignEntity::VARIABLE) {
              throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for pattern argument 1");
          }
      }
  }
}

//checks that with clause contain comparison of same type of attribute
void SemanticValidator::checkWithClauseSameAttributeCompare() {
  std::vector<WithClause> withClauses = getQuery().getWithClause();
  std::vector<Declaration> declr = getQuery().getDeclarations();
  for (auto & withClause : withClauses) {
    if (withClause.getRef1().attrType != withClause.getRef2().attrType) {
      throw QueryException(ErrorType::Semantic, "Semantic error. Comparison of different type of attribute in with clause");
    }
  }
}

//checks that attrRef have valid attrName
void SemanticValidator::checkAttrRefValidAttrName() {
  std::vector<Declaration> declr = getQuery().getDeclarations();
  Result result = getQuery().getSelectClause();
  if (std::holds_alternative<std::vector<Element>>(result)) {
    std::vector<Element> tuple = std::get<std::vector<Element>>(result);
    for (auto & ele : tuple) {
      if (std::holds_alternative<AttrRef>(ele)) {
        checkAttrRefValidAttrNameHelper(std::get<AttrRef>(ele), declr);
      }
    }
  }
  std::vector<WithClause> withClauses = getQuery().getWithClause();
  for (auto & clau : withClauses) {
    WithRef ref1 = clau.getRef1();
    WithRef ref2 = clau.getRef2();
    if (std::holds_alternative<AttrRef>(ref1.ref)) {
      checkAttrRefValidAttrNameHelper(std::get<AttrRef>(ref1.ref), declr);
    }
    if (std::holds_alternative<AttrRef>(ref2.ref)) {
      checkAttrRefValidAttrNameHelper(std::get<AttrRef>(ref2.ref), declr);
    }
  }
}

// HELPER FUNCTIONS

//Check Synonym got Declaration
void SemanticValidator::checkSynonymDeclareHelper(Ref reff, std::vector<Declaration>& declr, const std::string& missing) {
  if (std::holds_alternative<Synonym>(reff)) {
    if (!static_cast<bool>(Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(reff)).has_value())) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in " + missing);
    }
  }
}

//Check Synonym in Element got Declaration
void SemanticValidator::checkElementSynonymDeclareHelper(Element ele, std::vector<Declaration>& declr) {
  if (std::holds_alternative<Synonym>(ele)) {
    if (!Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(ele)).has_value()) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in Select clause for synonym " +
        std::get<Synonym>(ele).getSynonym());
    }
  }
  else if (std::holds_alternative<AttrRef>(ele)) {
    if (!Declaration::findDeclarationWithSynonym(declr, std::get<AttrRef>(ele).synonym).has_value()) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in Select clause for synonym (in AttrRef) " +
        std::get<AttrRef>(ele).synonym.getSynonym());
    }
  }
}

//Check Synonym in WithRef got Declaration
void SemanticValidator::checkWithRefSynonymDeclareHelper(WithRef reff, std::vector<Declaration>& declr) {
  if (std::holds_alternative<AttrRef>(reff.ref)) {
    if (!Declaration::findDeclarationWithSynonym(declr, std::get<AttrRef>(reff.ref).synonym).has_value()) {
      throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration for synonym (in WithRef) " +
        std::get<AttrRef>(reff.ref).synonym.getSynonym());
    }
  }
}

//Check correct statement design entity
void SemanticValidator::checkSynonymStatementHelper(DesignEntity des, const std::string& relStr) {
  if (des != DesignEntity::STMT && des != DesignEntity::READ && des != DesignEntity::PRINT && des != DesignEntity::ASSIGN
    && des != DesignEntity::IF && des != DesignEntity::WHILE && des != DesignEntity::CALL) {
    throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
  }
}

//Check correct design entity of second synonym in Relationship
void SemanticValidator::checkSecondSynonymHelper(DesignEntity des2, const std::string& relStr, Relationship rel) {
  if ((rel == Relationship::Calls || rel == Relationship::CallsT)) {
    if (des2 != DesignEntity::PROCEDURE) {
      throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
    }
  }
  else if ((rel == Relationship::ModifiesP || rel == Relationship::UsesP || rel == Relationship::ModifiesS
    || rel == Relationship::UsesS)) {
    if (des2 != DesignEntity::VARIABLE) {
      throw QueryException(ErrorType::Semantic, "Semantic error. Wrong design entity type for " + relStr);
    }
  }
  else {
    checkSynonymStatementHelper(des2, relStr);
  }
}

//Check Attribute Name is valid with given synonym
void SemanticValidator::checkAttrRefValidAttrNameHelper(AttrRef attr, std::vector<Declaration>& declr) {
  if (Declaration::findDeclarationWithSynonym(declr, attr.synonym).has_value()) {
    Declaration decl = Declaration::findDeclarationWithSynonym(declr, attr.synonym).value();
    std::set<AttrName> validAttrNameSet = getValidAttrNameSet(decl);
    if (validAttrNameSet.find(attr.attrName) == validAttrNameSet.end()) {
      throw QueryException(ErrorType::Semantic, "Semantic error. Attribute name is invalid with synonym " + attr.synonym.getSynonym());
    }
  }
}

}  // namespace qps
