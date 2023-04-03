#include "query/with_clause.h"

#include <utility>

namespace qps {

WithClause::WithClause(WithRef ref1_, WithRef ref2_) : ref1{std::move( ref1_ )}, ref2{std::move( ref2_ )} {}

auto WithClause::getRef1() -> WithRef {
	return ref1;
}

auto WithClause::getRef2() -> WithRef {
	return ref2;
}

}  // namespace qps
