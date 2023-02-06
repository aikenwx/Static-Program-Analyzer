#include "var_relationship.h"

namespace rel {
std::string VarRelationship::variableName() { return variableName_; }

VarRelationship VarRelationship::CreateRelationship(std::string variableName) {
  return VarRelationship(variableName);
};

VarRelationship::VarRelationship(std::string variableName) {
  variableName_ = variableName;
};
}  // namespace rel
