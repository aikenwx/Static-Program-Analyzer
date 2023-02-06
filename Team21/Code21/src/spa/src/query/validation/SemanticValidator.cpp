#include <unordered_set>
#include <string>
#include <variant>

#include "SemanticValidator.h"

namespace qps {

	SemanticValidator::SemanticValidator(Query query) :QueryValidator(query) {}

	bool SemanticValidator::validateQuery() {
		checkForDuplicateDeclarations();
		checkIfSynonymContainedInDeclaration();
		checkPatternClauseSynAssign();
		checkNoWildCardFirstArgModifiesUses();
		checkRelationSynonymMatchDesignEntity();
		return true;
	}

	void SemanticValidator::checkForDuplicateDeclarations() {
		//create a hashset and then add to it. If current iterable element is found in hashset, it means duplicate.
		std::unordered_set<std::string> seen;
		for (auto & declr : getQuery().getDeclarations()) {
			std::string syn = declr.getSynonym().getSynonym();
			if (seen.find(syn) == seen.end()) {
				seen.insert(syn);
			}
			else {
				throw QueryException(ErrorType::Semantic, "Semantic error. There is duplicate declaration found for " + syn);
			}
		}
	}

	void SemanticValidator::checkIfSynonymContainedInDeclaration() {
		std::vector<Declaration> declr = getQuery().getDeclarations();
		Synonym syn = getQuery().getSelectClause().getSynonym();
		if (Declaration::findDeclarationWithSynonym(declr, syn).has_value() == false) {
			throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in Select clause for " + syn.getSynonym());
		}
		if (!getQuery().getSuchThatClause().empty()) {
			Ref refSuchThat1 = getQuery().getSuchThatClause()[0].getArg1();
			Ref refSuchThat2 = getQuery().getSuchThatClause()[0].getArg2();
			if (std::holds_alternative<Synonym>(refSuchThat1)) {
				if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(refSuchThat1)).has_value() == false) {
					throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in SuchThat clause for argument 1");
				}
			}
			if (std::holds_alternative<Synonym>(refSuchThat2)) {
				if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(refSuchThat2)).has_value() == false) {
					throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in SuchThat clause for argument 2");
				}
			}
		}
		if (!getQuery().getPatternClause().empty()) {
			Ref refPattern = getQuery().getPatternClause()[0].getArg1();
			if (std::holds_alternative<Synonym>(refPattern)) {
				if (Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(refPattern)).has_value() == false) {
					throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in AssignPattern clause for argument 1");
				}
			}
		}
	}

	void SemanticValidator::checkNoWildCardFirstArgModifiesUses() {

	}

	void SemanticValidator::checkPatternClauseSynAssign() {
		//std::vector<Declaration> declr = getQuery().getDeclarations();
		//if (!getQuery().getPatternClause().empty()) {
		//	Ref refPattern = getQuery().getPatternClause()[0].getAssign();
		//	std::optional<Declaration> patternDclr = Declaration::findDeclarationWithSynonym(declr, std::get<Synonym>(refPattern));
		//	if (patternDclr.has_value()) {
		//		if (patternDclr.value().getDesignEntity() != DesignEntity::ASSIGN) {
		//			throw QueryException(ErrorType::Semantic, "Semantic error. There is missing declaration in AssignPattern clause for argument 1");
		//		}
		//	}
		//}
	}

	void SemanticValidator::checkRelationSynonymMatchDesignEntity() {

	}
}
