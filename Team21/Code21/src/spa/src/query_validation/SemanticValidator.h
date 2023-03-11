#pragma once

#include "query/query_exceptions.h"

#include "QueryValidator.h"

namespace qps {

	class SemanticValidator : public QueryValidator {
	public:
		SemanticValidator(Query query);

		//checks query by calling all the other check functions
		bool validateQuery();

		//checks for no duplicate declarations
		void checkForDuplicateDeclarations();

		//checks that all synonyms in clauses are from declarations
		void checkIfSynonymContainedInDeclaration();

		void checkSynonymDeclareHelper(Ref r, std::vector<Declaration>& declr, std::string missing);

		void checkElementSynonymDeclareHelper(Element r, std::vector<Declaration>& declr);

		void checkWithRefSynonymDeclareHelper(WithRef r, std::vector<Declaration>& declr);

		//check that the pattern clause has a synonym with valid design entity
		void checkPatternClauseSynonym();

		//checks that no wild card as first argument for modifies and uses
		void checkNoWildCardFirstArgModifiesUses();

		//checks that synonyms declared in such that clause for relationships(e.g. parent) have the correct design entity types for that relation
		void checkRelationSynonymMatchDesignEntity();

		void checkSynonymStatementHelper(DesignEntity d, std::string relStr);

		//checks that pattern first arg is variable if it is synonym
		void checkPatternSynonymMatchDesignEntity();

		//checks that with clause contain comparison of same type of attribute
		void checkWithClauseSameAttributeCompare();

		//checks that attrRef have valid attrName
		void checkAttrRefValidAttrName();

		void checkAttrRefValidAttrNameHelper(AttrRef ar, std::vector<Declaration>& declr);
	};
}
