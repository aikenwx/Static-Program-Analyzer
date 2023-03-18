#pragma once

#include "query/query_exceptions.h"

#include "QueryValidator.h"

namespace qps {

	class SemanticValidator : public QueryValidator {
	public:
		explicit SemanticValidator(Query& query);

		//checks query by calling all the other check functions
		auto validateQuery() -> bool override;

		//checks for no duplicate declarations
		void checkForDuplicateDeclarations();

		//checks that all synonyms in clauses are from declarations
		void checkIfSynonymContainedInDeclaration();

		//check that the pattern clause has a synonym with valid design entity
		void checkPatternClauseSynonym();

		//checks that no wild card as first argument for modifies and uses
		void checkNoWildCardFirstArgModifiesUses();

		//checks that synonyms declared in such that clause for relationships(e.g. parent) have the correct design entity types for that relation
		void checkRelationSynonymMatchDesignEntity();

		//checks that pattern first arg is variable if it is synonym
		void checkPatternSynonymMatchDesignEntity();

		//checks that with clause contain comparison of same type of attribute
		void checkWithClauseSameAttributeCompare();

		//checks that attrRef have valid attrName
		void checkAttrRefValidAttrName();

	private:
		static void checkSynonymDeclareHelper(Ref reff, std::vector<Declaration>& declr, const std::string &missing);

		static void checkElementSynonymDeclareHelper(Element ele, std::vector<Declaration>& declr);

		static void checkWithRefSynonymDeclareHelper(WithRef reff, std::vector<Declaration>& declr);

		static void checkSynonymStatementHelper(DesignEntity des, const std::string &relStr);

		static void checkAttrRefValidAttrNameHelper(AttrRef attr, std::vector<Declaration>& declr);
	};
}  // namespace qps
