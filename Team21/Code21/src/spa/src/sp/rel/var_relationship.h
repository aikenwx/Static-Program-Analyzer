#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class VarRelationship : public Relationship {
 public:
  static auto CreateRelationship(const std::string& variableName) -> std::unique_ptr<VarRelationship>;
  auto variableName() const -> std::string;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::VAR; };

 private:
  explicit VarRelationship(std::string_view variableName) : variableName_(variableName) {};
  std::string variableName_;
};
}  // namespace rel
