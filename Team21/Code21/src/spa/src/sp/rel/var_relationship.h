#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class VarRelationship : public Relationship {
 public:
  static std::unique_ptr<VarRelationship> CreateRelationship(std::string variableName);
  std::string variableName();
  RelationshipType relationshipType() override { return RelationshipType::VAR; };

 private:
  VarRelationship(std::string variableName);
  std::string variableName_;
};
}  // namespace rel
