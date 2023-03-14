#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ConstRelationship : public Relationship {
 public:
  static std::unique_ptr<ConstRelationship> CreateRelationship(int value);
  int value() const;
  RelationshipType relationshipType() const override { return RelationshipType::CONST; };

 private:
  ConstRelationship(int value);
  int value_;
};
} // namespace rel
