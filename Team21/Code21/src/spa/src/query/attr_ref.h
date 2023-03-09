#pragma once

#include "query/synonym.h"
#include "query/declaration.h"
#include "query/synonym.h"
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

AttrType getAttrType(AttrName attrName);
AttrName getAttrNameFromString(std::string str);
std::set<AttrName> getValidAttrNameSet(Declaration declaration);

class AttrRef {
	public:
		Synonym synonym;
		AttrName attrName;

		AttrRef(Synonym synonym_, AttrName attrName_);

		bool operator==(const AttrRef& other) const {
			return synonym == other.synonym && attrName == other.attrName;
		}
};
}
