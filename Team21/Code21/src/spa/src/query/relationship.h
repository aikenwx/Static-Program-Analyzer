#pragma once

#include <vector>
#include <unordered_map>

#include "query/ref.h"
#include "design_entity.h"

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
  Calls,
  CallsT,
  Next,
  NextT,
  Affects,
  AffectsT
};

extern const std::unordered_map<Relationship, DesignEntity> AllowedDesignEntityOnLeft;

extern const std::unordered_map<Relationship, DesignEntity> AllowedDesignEntityOnRight;

auto getRelationshipFromString(const std::string &reString) -> Relationship;
auto getStringFromRelationship(Relationship relationship) -> std::string;

}  // namespace qps
