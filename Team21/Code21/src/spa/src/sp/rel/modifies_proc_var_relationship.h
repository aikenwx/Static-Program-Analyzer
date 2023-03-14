#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesProcVarRelationship : public ProcVarRelationship {
 public:
  static auto CreateRelationship(
      const std::string& procedureName, const std::string& variableName) -> std::unique_ptr<ModifiesProcVarRelationship>;
  auto procedureName() const -> std::string override;
  auto variableName() const -> std::string override;
  auto relationshipType() const -> RelationshipType override {
    return RelationshipType::MODIFIES_PROC_VAR;
  };

 private:
  ModifiesProcVarRelationship(const std::string& procedureName,
                              const std::string& variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
