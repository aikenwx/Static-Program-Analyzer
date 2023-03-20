#pragma once

#include "query/synonym.h"
#include "query/declaration.h"
#include "query/query_exceptions.h"


#include <variant>
#include <set>
#include <stdexcept>
#include <map>

namespace qps {

enum class AttrName {
	ProcName,
	VarName,
	Value,
	StmtNo,
};

enum class AttrType {
	NAME,
	INTEGER,
};

auto getAttrType(AttrName attrName) -> AttrType;
auto getAttrNameFromString(const std::string& str) -> AttrName;
auto getValidAttrNameSet(Declaration declaration) -> std::set<AttrName>;

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
