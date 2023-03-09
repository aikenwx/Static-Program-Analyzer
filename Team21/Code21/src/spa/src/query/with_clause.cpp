#include "query/with_clause.h"

namespace qps {

	WithClause::WithClause(WithRef ref1_, WithRef ref2_) : ref1{ ref1_ }, ref2{ ref2_ } {
		if (ref1.attrType != ref2.attrType) {
			throw QueryException(ErrorType::Semantic, "Comparison of different type of attribute in with clause");
		}
	}

	WithRef WithClause::getRef1() {
		return ref1;
	}

	WithRef WithClause::getRef2() {
		return ref2;
	}
}