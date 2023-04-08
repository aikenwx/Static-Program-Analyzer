#include "uses_proc_var_relationship.h"

#include <string>

namespace rel {
auto UsesProcVarRelationship::CreateRelationship(
    const std::string &procedureName, const std::string &variableName)
    -> std::unique_ptr<UsesProcVarRelationship> {
  return std::unique_ptr<UsesProcVarRelationship>(
      new UsesProcVarRelationship(procedureName, variableName));
};
} // namespace rel
