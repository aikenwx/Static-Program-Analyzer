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
	}

	void SemanticValidator::checkForDuplicateDeclarations() {
		//create a hashset and then add to it. If current iterable element is found in hashset, it means duplicate.
		std::unordered_set<Declaration> seen;
		for (auto & declr : query_.getDeclarations()) {
			std::string syn = declr.getSynonym().getSynonym();
			if (seen.find(syn) == seen.end()) {
				seen.insert(syn);
			}
			else {
				throw QueryException("There is duplictate declaration found for" + syn);
			}
		}
	}

	//Does the findDeclarationsWithSynonym work?
	void SemanticValidator::checkIfSynonymContainedInDeclaration() {
		std::vector<Declaration>& declr = query_.getDeclarations();
		std::string syn = query_.getSelectClause().getSynonym();
		if (Declaration::findDeclarationWithSynonym(declr, syn) == false) {
			throw QueryException("There is missing declaration in select clause for" + syn);
		}
		if (query_.getSuchThatClause() != NULL) {
			Ref refSuchThat1 = query_.getSuchThatClause().getArg1();
			Ref refSuchThat2 = query_.getSuchThatClause().getArg2();
			if (std::holds_alternative<Synonym>(refSuchThat1)) {
				if (Declaration::findDeclarationWithSynonym(declr, refSuchThat1.getSynonym()) == false) {
					throw QueryException("There is missing declaration in SuchThat clause for argument 1);
				}
			}
			if (std::holds_alternative<Synonym>(refSuchThat2)) {
				if (Declaration::findDeclarationWithSynonym(declr, refSuchThat2.getSynonym()) == false) {
					throw QueryException("There is missing declaration in SuchThat clause for argument 2);
				}
			}
		}
		if (query_.getPatternClause() != NULL) {
			Ref refPattern = query_.getPatternClause().getArg1();
			if (std::holds_alternative<Synonym>(refPattern)) {
				if (Declaration::findDeclarationWithSynonym(declr, refPattern.getSynonym()) == false) {
					throw QueryException("There is missing declaration in AssignPattern clause for argument 1);
				}
			}
		}
	}

	void SemanticValidator::checkNoWildCardFirstArgModifiesUses() {

	}

	void SemanticValidator::checkPatternClauseSynAssign() {

	}

	void SemanticValidator::checkRelationSynonymMatchDesignEntity() {

	}
}
