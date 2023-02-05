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

Relationship getRelationshipFromString(std::string& reString);
std::string getStringFromRelationship(Relationship relationship);

}

