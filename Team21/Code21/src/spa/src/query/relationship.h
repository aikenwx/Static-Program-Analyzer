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

const std::unordered_map<Relationship, DesignEntity>
    AllowedDesignEntityOnLeft = {{Relationship::Follows, DesignEntity::STMT},
                                 {Relationship::FollowsT, DesignEntity::STMT},
                                 {Relationship::Parent, DesignEntity::STMT},
                                 {Relationship::ParentT, DesignEntity::STMT},
                                 {Relationship::UsesS, DesignEntity::STMT},
                                 {Relationship::UsesP, DesignEntity::PROCEDURE},
                                 {Relationship::ModifiesS, DesignEntity::STMT},
                                 {Relationship::ModifiesP, DesignEntity::PROCEDURE},
                                 {Relationship::Calls, DesignEntity::PROCEDURE},
                                 {Relationship::CallsT, DesignEntity::PROCEDURE},
                                 {Relationship::Next, DesignEntity::STMT},
                                 {Relationship::NextT, DesignEntity::STMT},
                                 {Relationship::Affects, DesignEntity::STMT},
                                 {Relationship::AffectsT, DesignEntity::STMT}
};

const std::unordered_map<Relationship, DesignEntity>
    AllowedDesignEntityOnRight = {{Relationship::Follows, DesignEntity::STMT},
                                  {Relationship::FollowsT, DesignEntity::STMT},
                                  {Relationship::Parent, DesignEntity::STMT},
                                  {Relationship::ParentT, DesignEntity::STMT},
                                  {Relationship::UsesS, DesignEntity::VARIABLE},
                                  {Relationship::UsesP, DesignEntity::VARIABLE},
                                  {Relationship::ModifiesS, DesignEntity::VARIABLE},
                                  {Relationship::ModifiesP, DesignEntity::VARIABLE},
                                  {Relationship::Calls, DesignEntity::PROCEDURE},
                                  {Relationship::CallsT, DesignEntity::PROCEDURE},
                                  {Relationship::Next, DesignEntity::STMT},
                                  {Relationship::NextT, DesignEntity::STMT},
                                  {Relationship::Affects, DesignEntity::STMT},
                                  {Relationship::AffectsT, DesignEntity::STMT}
};

auto getRelationshipFromString(const std::string &reString) -> Relationship;
auto getStringFromRelationship(Relationship relationship) -> std::string;

}  // namespace qps
