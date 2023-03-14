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
  return std::unique_ptr<ModifiesProcVarRelationship>(new ModifiesProcVarRelationship(std::move(procedureName), std::move(variableName)));
};

ModifiesProcVarRelationship::ModifiesProcVarRelationship(const std::string& procedureName, const std::string& variableName) {
  procedureName_ = procedureName;
  variableName_ = variableName;
}
}  // namespace rel
