#pragma once

#include "memory"

#include "query/pattern_clause.h"
#include "pattern_evaluator.h"

namespace qps {

	class PatternEvaluatorFactory {
	public:
		static auto Create(PatternClause& clause, std::vector<Declaration>& decl_lst) -> std::unique_ptr<PatternEvaluator>;
	};

} // namespace qps
