#pragma once

#include "query/synonym.h"
#include "query/attr_ref.h"

#include <variant>

namespace qps {

	using Element = std::variant<Synonym, AttrRef>;
}
