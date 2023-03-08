#include "query/query.h"

namespace qps {

	Query::Query(std::vector<Declaration> declarations_, std::vector<SuchThatClause> suchThatClause_,
		std::vector<PatternClause> patternClause_, Result selectClause_)
		: declarations{ declarations_ }, suchThatClause{suchThatClause_},
		patternClause{ patternClause_ }, selectClause{ selectClause_ } {}

	Query::Query(Result selectClause_) : selectClause{ selectClause_ } {}

	std::vector<Declaration> Query::getDeclarations() {
		return declarations;
	}
	std::vector<SuchThatClause> Query::getSuchThatClause() {
		return suchThatClause;
	}
	std::vector<PatternClause> Query::getPatternClause() {
		return patternClause;
	}
	Result Query::getSelectClause() {
		return selectClause;
	}
}
