#include "const_relationship.h"

namespace rel {
int ConstRelationship::value() { return value_; }

ConstRelationship* ConstRelationship::CreateRelationship(int value) {
  return new ConstRelationship(value);
};

ConstRelationship::ConstRelationship(int value) { value_ = value; };
} // namespace rel
