#include <utility>

#include "query/query.h"

namespace qps {

	Query::Query(std::vector<Declaration> declarations_, std::vector<SuchThatClause> suchThatClause_,
		std::vector<PatternClause> patternClause_, std::vector<WithClause> withClause_, Result selectClause_)
		: declarations{std::move( declarations_ )}, suchThatClause{std::move(suchThatClause_)},
		patternClause{std::move( patternClause_ )}, withClause{std::move( withClause_ )}, selectClause{std::move( selectClause_ )} {}

	Query::Query(Result selectClause_) : selectClause{ std::move(selectClause_) } {}

	auto Query::getDeclarations() -> std::vector<Declaration> {
		return declarations;
	}
	auto Query::getSuchThatClause() -> std::vector<SuchThatClause> {
		return suchThatClause;
	}
	auto Query::getPatternClause() -> std::vector<PatternClause> {
		return patternClause;
	}
	auto Query::getWithClause() -> std::vector<WithClause> {
		return withClause;
	}
	auto Query::getSelectClause() -> Result {
		return selectClause;
	}
}  // namespace qps
