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
		SuchThatClause suchThatClause;
		PatternClause patternClause;
		Declaration selectClause;

	public:
		Query(std::vector<Declaration>& declarations_, SuchThatClause suchThatClause_,
			PatternClause patternClause_, Declaration selectClause_);
		std::vector<Declaration>& getDeclarations();
		SuchThatClause getSuchThatClause();
		PatternClause getPatternClause();
		Declaration getSelectClause();
};
}

