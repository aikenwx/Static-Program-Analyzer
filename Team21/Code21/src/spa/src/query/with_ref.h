#pragma once

#include "query/attr_ref.h"
#include "query/quoted_identifier.h"

#include <variant>

namespace qps {

	using WithRefType = std::variant<QuotedIdentifier, int, AttrRef>;

	class WithRef {
		public:
			WithRefType ref;
			AttrType attrType;

			WithRef(WithRefType ref_);

			bool operator==(const WithRef& other) const {
				return ref == other.ref && attrType == other.attrType;
			}
	};
}
