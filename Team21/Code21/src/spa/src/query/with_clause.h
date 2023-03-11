#pragma once

#include "query/with_ref.h"

#include <optional>
#include <variant>

namespace qps {

	class WithClause {
	private:
		WithRef ref1;
		WithRef ref2;

	public:
		WithClause(WithRef ref1_, WithRef ref2_);
		WithRef getRef1();
		WithRef getRef2();

		bool operator==(const WithClause& clause) const {
			return ref1 == clause.ref1 && ref2 == clause.ref2;
		}
	};

}
