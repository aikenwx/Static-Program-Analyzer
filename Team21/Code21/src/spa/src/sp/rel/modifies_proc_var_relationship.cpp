#include "modifies_proc_var_relationship.h"

#include <string>

namespace rel {
std::string ModifiesProcVarRelationship::procedureName() {
  return procedureName_;
};

std::string ModifiesProcVarRelationship::variableName() {
  return variableName_;
};

std::unique_ptr<ModifiesProcVarRelationship>
ModifiesProcVarRelationship::CreateRelationship(std::string procedureName,
                                                std::string variableName) {
  return std::unique_ptr<ModifiesProcVarRelationship>(new ModifiesProcVarRelationship(procedureName, variableName));
};

ModifiesProcVarRelationship::ModifiesProcVarRelationship(std::string procedureName, std::string variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
