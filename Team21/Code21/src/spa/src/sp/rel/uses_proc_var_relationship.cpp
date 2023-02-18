#include "uses_proc_var_relationship.h"

#include <string>

namespace rel {
std::string UsesProcVarRelationship::procedureName() { return procedureName_; };

std::string UsesProcVarRelationship::variableName() { return variableName_; };

std::unique_ptr<UsesProcVarRelationship> UsesProcVarRelationship::CreateRelationship(
    std::string procedureName, std::string variableName) {
  return std::unique_ptr<UsesProcVarRelationship>(new UsesProcVarRelationship(procedureName, variableName));
};

UsesProcVarRelationship::UsesProcVarRelationship(std::string procedureName,
                                                 std::string variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
