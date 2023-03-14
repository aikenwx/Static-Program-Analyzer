#include "modifies_proc_var_relationship.h"

#include <string>
#include <utility>

namespace rel {
auto ModifiesProcVarRelationship::procedureName() const -> std::string {
  return procedureName_;
};

auto ModifiesProcVarRelationship::variableName() const -> std::string {
  return variableName_;
};

auto
ModifiesProcVarRelationship::CreateRelationship(const std::string& procedureName,
                                                const std::string& variableName) -> std::unique_ptr<ModifiesProcVarRelationship> {
  return std::unique_ptr<ModifiesProcVarRelationship>(new ModifiesProcVarRelationship(procedureName, variableName));
};

ModifiesProcVarRelationship::ModifiesProcVarRelationship(std::string_view procedureName, std::string_view variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
