#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesProcVarRelationship : public ProcVarRelationship {
 public:
  static std::unique_ptr<ModifiesProcVarRelationship> CreateRelationship(
      std::string procedureName, std::string variableName);
  std::string procedureName() const override;
  std::string variableName() const override;
  RelationshipType relationshipType() const override {
    return RelationshipType::MODIFIES_PROC_VAR;
  };

 private:
  ModifiesProcVarRelationship(std::string procedureName,
                              std::string variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
