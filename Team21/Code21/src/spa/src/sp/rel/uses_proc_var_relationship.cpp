#include "uses_proc_var_relationship.h"

#include <string>

namespace rel {
auto UsesProcVarRelationship::procedureName() const -> std::string { return procedureName_; };

auto UsesProcVarRelationship::variableName() const -> std::string { return variableName_; };

auto UsesProcVarRelationship::CreateRelationship(
    const std::string& procedureName, const std::string& variableName) -> std::unique_ptr<UsesProcVarRelationship> {
  return std::unique_ptr<UsesProcVarRelationship>(new UsesProcVarRelationship(procedureName, variableName));
};
}  // namespace rel
