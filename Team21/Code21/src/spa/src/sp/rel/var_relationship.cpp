#include "var_relationship.h"

namespace rel {
auto VarRelationship::variableName() const -> std::string { return variableName_; }

auto VarRelationship::CreateRelationship(const std::string& variableName) -> std::unique_ptr<VarRelationship> {
  return std::unique_ptr<VarRelationship>(new VarRelationship(variableName));
};

VarRelationship::VarRelationship(std::string_view variableName) {
  variableName_ = variableName;
};
}  // namespace rel
