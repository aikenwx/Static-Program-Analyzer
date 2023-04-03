#pragma once

#include "query/attr_ref.h"
#include "query/quoted_identifier.h"

#include <variant>

namespace qps {

// WithRefType represents With Reference type used in With Clause
using WithRefType = std::variant<QuotedIdentifier, int, AttrRef>;

// WithRef represents With Reference used in With Clause
class WithRef {
 public:
	WithRefType ref;
	AttrType attrType;

	explicit WithRef(WithRefType ref_);

	auto operator==(const WithRef& other) const -> bool {
		return ref == other.ref && attrType == other.attrType;
	}
};

}  // namespace qps
