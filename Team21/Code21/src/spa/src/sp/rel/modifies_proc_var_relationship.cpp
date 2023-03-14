#include "modifies_proc_var_relationship.h"

#include <string>
#include <utility>

namespace rel {
std::string ModifiesProcVarRelationship::procedureName() const {
  return procedureName_;
};

std::string ModifiesProcVarRelationship::variableName() const {
  return variableName_;
};

std::unique_ptr<ModifiesProcVarRelationship>
ModifiesProcVarRelationship::CreateRelationship(const std::string& procedureName,
                                                const std::string& variableName) {
  return std::unique_ptr<ModifiesProcVarRelationship>(new ModifiesProcVarRelationship(std::move(procedureName), std::move(variableName)));
};

ModifiesProcVarRelationship::ModifiesProcVarRelationship(const std::string& procedureName, const std::string& variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
