#pragma once

#include "query/declaration.h"
#include "query/pattern_clause.h"
#include "query/such_that_clause.h"

#include <vector>

namespace qps {

	using Clause = std::variant<qps::SuchThatClause, qps::PatternClause>;

class Query {
	private:
		std::vector<Declaration>& declarations;
		std::vector<SuchThatClause>& suchThatClause;
		std::vector<PatternClause>& patternClause;
		Declaration selectClause;

	public:
		Query(std::vector<Declaration>& declarations_, std::vector<SuchThatClause>& suchThatClause_,
			std::vector<PatternClause>& patternClause_, Declaration selectClause_);
		Query(Declaration selectClause_);
		std::vector<Declaration>& getDeclarations();
		std::vector<SuchThatClause>& getSuchThatClause();
		std::vector<PatternClause>& getPatternClause();
		Declaration getSelectClause();
};
}

