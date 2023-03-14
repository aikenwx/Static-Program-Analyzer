#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ConstRelationship : public Relationship {
 public:
  static auto CreateRelationship(int value) -> std::unique_ptr<ConstRelationship>;
  auto value() const -> int;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::CONST; };

 private:
  ConstRelationship(int value);
  int value_;
};
} // namespace rel
