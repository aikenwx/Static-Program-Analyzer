#include "uses_proc_var_relationship.h"

#include <string>

namespace rel {
std::string UsesProcVarRelationship::procedureName() const { return procedureName_; };

std::string UsesProcVarRelationship::variableName() const { return variableName_; };

std::unique_ptr<UsesProcVarRelationship> UsesProcVarRelationship::CreateRelationship(
    const std::string& procedureName, const std::string& variableName) {
  return std::unique_ptr<UsesProcVarRelationship>(new UsesProcVarRelationship(procedureName, variableName));
};

UsesProcVarRelationship::UsesProcVarRelationship(std::string_view procedureName,
                                                 std::string_view variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
