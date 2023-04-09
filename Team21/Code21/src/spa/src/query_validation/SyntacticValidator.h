#pragma once

#include "query/query_exceptions.h"
#include "QueryValidator.h"

#include <regex>

namespace qps {

class SyntacticValidator : public QueryValidator {
 public:
	explicit SyntacticValidator(Query& query);
	auto validateQuery() -> bool override;
	void checkSuchThatCorrectRefTypes();
	void checkPatternCorrectRefTypes();
};

}  // namespace qps
