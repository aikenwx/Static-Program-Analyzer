#include "const_relationship.h"

namespace rel {
auto ConstRelationship::value() const -> int { return value_; }

auto ConstRelationship::CreateRelationship(int value) -> std::unique_ptr<ConstRelationship> {
  return std::unique_ptr<ConstRelationship>(new ConstRelationship(value));
};

ConstRelationship::ConstRelationship(int value) { value_ = value; };
} // namespace rel
