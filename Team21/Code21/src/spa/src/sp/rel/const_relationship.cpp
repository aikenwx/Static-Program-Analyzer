#include "const_relationship.h"

namespace rel {
int ConstRelationship::value() { return value_; }

std::unique_ptr<ConstRelationship> ConstRelationship::CreateRelationship(int value) {
  return std::unique_ptr<ConstRelationship>(new ConstRelationship(value));
};

ConstRelationship::ConstRelationship(int value) { value_ = value; };
} // namespace rel
