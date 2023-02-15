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

		//check that the pattern clause has a syn-assign that is a pattern declaration
		void checkPatternClauseSynAssign();

		//checks that no wild card as first argument for modifies and uses
		void checkNoWildCardFirstArgModifiesUses();

		//checks that synonyms declared in such that clause for relationships(e.g. parent) have the correct design entity types for that relation
		void checkRelationSynonymMatchDesignEntity();

	};
}
