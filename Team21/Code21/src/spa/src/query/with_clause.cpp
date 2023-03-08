#include "query/with_clause.h"

namespace qps {

	WithClause::WithClause(WithRef ref1, WithRef ref2) : ref1{ ref1_ }, ref2{ ref2_ } {
		if (ref1.attrType != ref2.attrType) {
			throw QueryException(ErrorType::Semantic, "Comparison of different type of attribute in with clause");
		}
	}

	AttrRef WithClause::getRef1() {
		return ref1;
	}

	AttrRef WithClause::getRef2() {
		return ref2;
	}
}