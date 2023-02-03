#include "query/query.h"

namespace qps {

	Query::Query(std::vector<Declaration>& declarations_, SuchThatClause suchThatClause_,
		PatternClause patternClause_, Declaration selectClause_)
		: declarations{ declarations_ }, suchThatClause{suchThatClause_},
		patternClause{ patternClause_ }, selectClause{ selectClause_ } {}

	std::vector<Declaration>& Query::getDeclarations() {
		return declarations;
	}
	SuchThatClause Query::getSuchThatClause() {
		return suchThatClause;
	}
	PatternClause Query::getPatternClause() {
		return patternClause;
	}
	Declaration Query::getSelectClause() {
		return selectClause;
	}
}