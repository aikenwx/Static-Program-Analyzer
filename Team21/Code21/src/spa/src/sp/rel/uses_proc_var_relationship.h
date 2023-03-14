#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesProcVarRelationship : public ProcVarRelationship {
 public:
  static std::unique_ptr<UsesProcVarRelationship> CreateRelationship(
      const std::string& procedureName, const std::string& variableName);
  std::string procedureName() const override;
  std::string variableName() const override;
  RelationshipType relationshipType() const override { return RelationshipType::USES_PROC_VAR; };

 private:
  UsesProcVarRelationship(std::string_view procedureName, std::string_view variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
