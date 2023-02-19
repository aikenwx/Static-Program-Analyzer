#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesProcVarRelationship : public ProcVarRelationship {
 public:
  static std::unique_ptr<UsesProcVarRelationship> CreateRelationship(
      std::string procedureName, std::string variableName);
  std::string procedureName() override;
  std::string variableName() override;
  RelationshipType relationshipType() override { return RelationshipType::USES_PROC_VAR; };

 private:
  UsesProcVarRelationship(std::string procedureName, std::string variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
