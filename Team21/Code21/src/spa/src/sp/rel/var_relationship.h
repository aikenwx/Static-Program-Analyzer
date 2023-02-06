#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class VarRelationship : public Relationship {
 public:
  std::string variableName();
  static VarRelationship CreateRelationship(std::string variableName);

 private:
  VarRelationship(std::string variableName);
  std::string variableName_;
};
}  // namespace rel
