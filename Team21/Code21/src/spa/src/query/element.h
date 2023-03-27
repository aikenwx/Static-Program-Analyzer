#pragma once

#include "query/synonym.h"
#include "query/attr_ref.h"

#include <variant>

namespace qps {

	// Element represents element type used in Select Clause
	using Element = std::variant<Synonym, AttrRef>;
}
