#include <query/relationship.h>
#include "query/query_exceptions.h"

#include <stdexcept>
#include <map>

namespace qps {

const std::map<Relationship, std::string>
    relationshipStringMap{{
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
                              {Relationship::Calls, "Calls"},
                              {Relationship::CallsT, "Calls*"},
                              {Relationship::Next, "Next"},
                              {Relationship::NextT, "Next*"},
                              {Relationship::Affects, "Affects"},
                              {Relationship::AffectsT, "Affects*"}
                          }};

const std::map<std::string, Relationship>
    relationshipMap{{
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
                        {"Calls", Relationship::Calls},
                        {"Calls*", Relationship::CallsT},
                        {"Next", Relationship::Next},
                        {"Next*", Relationship::NextT},
                        {"Affects", Relationship::Affects},
                        {"Affects*", Relationship::AffectsT}
                    }};

auto getRelationshipFromString(const std::string &reString) -> Relationship {
  try {
    return relationshipMap.at(reString);
  }
  catch (const std::out_of_range &err) {
    throw QueryException(ErrorType::Syntactic, "Invalid Relationship" + reString);
  }
}
auto getStringFromRelationship(Relationship relationship) -> std::string {
  return relationshipStringMap.at(relationship);
}

}  // namespace qps
