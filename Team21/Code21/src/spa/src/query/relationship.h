#pragma once

#include "query/ref.h"

namespace qps {

enum class Relationship {
	Parent,
	ParentT,
	Follows,
	FollowsT,
	Uses,
	UsesS,
	UsesP,
	Modifies,
	ModifiesS,
	ModifiesP,
};

Relationship getRelationship(std::string&);
std::string getStringFromRelationship(Relationship);

}
