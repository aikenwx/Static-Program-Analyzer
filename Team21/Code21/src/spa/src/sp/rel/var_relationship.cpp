#include "var_relationship.h"

namespace rel {
std::string VarRelationship::variableName() { return variableName_; }

std::unique_ptr<VarRelationship> VarRelationship::CreateRelationship(std::string variableName) {
  return std::unique_ptr<VarRelationship>(new VarRelationship(variableName));
};

VarRelationship::VarRelationship(std::string variableName) {
  variableName_ = variableName;
};
}  // namespace rel
