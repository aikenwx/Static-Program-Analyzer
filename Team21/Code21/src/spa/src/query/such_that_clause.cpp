#include "query/such_that_clause.h"

namespace qps {

	Relationship SuchThatClause::getRelationship() {
		return relationship;
	}

	Ref SuchThatClause::getArg1() {
		return arg1;
	}

	Ref SuchThatClause::getArg2() {
		return arg2;
	}

	SuchThatClause::SuchThatClause(Relationship relationship_, Ref arg1_, Ref arg2_,
		std::vector<Declaration>& declarations)
		: relationship{ relationship_ }, arg1{ arg1_ }, arg2{ arg2_ } {
		handleModifiesUses(declarations);
	}
}
