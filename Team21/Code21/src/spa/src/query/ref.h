#pragma once

#include "query/synonym.h"
#include "query/quoted_identifier.h"
#include "query/underscore.h"

#include <variant>

namespace qps {

using StatementNumber = int;

// Ref represents a reference type used as argument in pattern and such that clause.
using Ref = std::variant<Synonym, Underscore, StatementNumber, QuotedIdentifier>;

}



