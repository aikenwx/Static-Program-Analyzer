#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ConstRelationship : public Relationship {
 public:
  static auto CreateRelationship(int value) -> std::unique_ptr<ConstRelationship>;
  [[nodiscard]] auto value() const -> int;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::CONST; };

 private:
  explicit ConstRelationship(int value) : value_(value) {};
  int value_;
};
} // namespace rel
