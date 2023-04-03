#pragma once

#include "query/synonym.h"
#include "query/declaration.h"
#include "query/query_exceptions.h"


#include <variant>
#include <set>
#include <stdexcept>
#include <map>

namespace qps {

// AttrName represents valid Attribute Name for Synonym
enum class AttrName {
	ProcName,
	VarName,
	Value,
	StmtNo,
};

// AttrType represents valid type of AttrName 
enum class AttrType {
	NAME,
	INTEGER,
};

auto getAttrType(AttrName attrName) -> AttrType;
auto getAttrNameFromString(const std::string& str) -> AttrName;
auto getValidAttrNameSet(Declaration declaration) -> std::set<AttrName>;

// AttrRef represents Attribute Reference to be used in With Clause and Select Clause
class AttrRef {
 public:
	Synonym synonym;
	AttrName attrName;

	AttrRef(Synonym synonym_, AttrName attrName_);

	auto operator==(const AttrRef& other) const -> bool {
		return synonym == other.synonym && attrName == other.attrName;
	}
};
}  // namespace qps
