#include "query/query.h"

namespace qps {

	Query::Query(std::vector<Declaration>& declarations_, std::vector<SuchThatClause>& suchThatClause_,
		std::vector<PatternClause>& patternClause_, Declaration selectClause_)
		: declarations{ declarations_ }, suchThatClause{suchThatClause_},
		patternClause{ patternClause_ }, selectClause{ selectClause_ } {}

	Query::Query(Declaration selectClause_)
		: declarations{}, suchThatClause{}, patternClause{}, selectClause { selectClause_ } {}

	std::vector<Declaration>& Query::getDeclarations() {
		return declarations;
	}
	std::vector<SuchThatClause>& Query::getSuchThatClause() {
		return suchThatClause;
	}
	std::vector<PatternClause>& Query::getPatternClause() {
		return patternClause;
	}
	Declaration Query::getSelectClause() {
		return selectClause;
	}
}
