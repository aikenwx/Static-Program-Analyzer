#pragma once

#include <iostream>

//Assumption on clauseresult
#include "query/Clause/ClauseResult.h"
#include "query/query.h"

namespace qps {
	class QueryEvaluator {
	private:
		Query query_;

	public:
		QueryEvaluator(Query query_);

		void evaluateQuery();

		ClauseResult selectClauseResult();

		ClauseResult suchThatClauseResult();

		ClauseResult assignPatternClauseResult();
	};
}
