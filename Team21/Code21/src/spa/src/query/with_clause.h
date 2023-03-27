#pragma once

#include "query/with_ref.h"

#include <optional>
#include <variant>

namespace qps {

	// WithClause represents Attribute Comparision in With Clause
	class WithClause {
	private:
		WithRef ref1;
		WithRef ref2;

	public:
		WithClause(WithRef ref1_, WithRef ref2_);
		auto getRef1() -> WithRef;
		auto getRef2() -> WithRef;

		auto operator==(const WithClause& clause) const -> bool {
			return ref1 == clause.ref1 && ref2 == clause.ref2;
		}
	};

}  // namespace qps
