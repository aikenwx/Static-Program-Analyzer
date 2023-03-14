#include "var_relationship.h"

namespace rel {
std::string VarRelationship::variableName() const { return variableName_; }

std::unique_ptr<VarRelationship> VarRelationship::CreateRelationship(const std::string& variableName) {
  return std::unique_ptr<VarRelationship>(new VarRelationship(variableName));
};

VarRelationship::VarRelationship(std::string_view variableName) {
  variableName_ = variableName;
};
}  // namespace rel
