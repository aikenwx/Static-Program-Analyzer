#pragma once

#include "query/ref.h"

namespace qps {

// Enum class Relationship represents all valid Relationships supported by Such That Clause
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
  Calls,
  CallsT,
  Next,
  NextT,
  Affects,
  AffectsT
};

auto getRelationshipFromString(const std::string& reString) -> Relationship;
auto getStringFromRelationship(Relationship relationship) -> std::string;

}  // namespace qps
