#include <query/relationship.h>
#include "query/query_exceptions.h"

#include <stdexcept>
#include <map>

namespace qps {

std::map<Relationship, std::string> relationshipStringMap{ {
	{Relationship::Follows, "Follows"},
	{Relationship::FollowsT, "Follows*"},
	{Relationship::Parent, "Parent"},
	{Relationship::ParentT, "Parent*"},
	{Relationship::Uses, "Uses"},
	{Relationship::UsesS, "Uses(S)"},
	{Relationship::UsesP, "Uses(P)"},
	{Relationship::Modifies, "Modifies"},
	{Relationship::ModifiesS, "Modifies(S)"},
	{Relationship::ModifiesP, "Modifies(P)"},
} };

std::map<std::string, Relationship> relationshipMap{ {
	{"Follows", Relationship::Follows},
	{"Follows*", Relationship::FollowsT},
	{"Parent", Relationship::Parent},
	{"Parent*", Relationship::ParentT},
	{"Uses", Relationship::Uses},
	{"Uses(S)", Relationship::UsesS},
	{"Uses(P)", Relationship::UsesP},
	{"Modifies", Relationship::Modifies},
	{"Modifies(S)", Relationship::ModifiesS},
	{"Modifies(P)", Relationship::ModifiesP},
} };

Relationship getRelationshipFromString(std::string reString) {
	try {
		return relationshipMap.at(reString);
	}
	catch (const std::out_of_range& err) {
		throw QueryException("Invalid Relationship" + reString);
	}
}
std::string getStringFromRelationship(Relationship relationship) {
	return relationshipStringMap.at(relationship);
}

}

