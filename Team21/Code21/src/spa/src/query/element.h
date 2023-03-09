#pragma once

#include "query/declaration.h"
#include "query/attr_ref.h"

#include <variant>

namespace qps {

	using Element = std::variant<Declaration, AttrRef>;
}
