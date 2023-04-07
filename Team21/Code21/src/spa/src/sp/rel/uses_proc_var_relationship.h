#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesProcVarRelationship : public ProcVarRelationship {
public:
  static auto CreateRelationship(const std::string &procedureName,
                                 const std::string &variableName)
      -> std::unique_ptr<UsesProcVarRelationship>;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override {
    return RelationshipType::USES_PROC_VAR;
  };

private:
  UsesProcVarRelationship(std::string_view procedureName,
                          std::string_view variableName)
      : ProcVarRelationship(procedureName, variableName){};
};
} // namespace rel
