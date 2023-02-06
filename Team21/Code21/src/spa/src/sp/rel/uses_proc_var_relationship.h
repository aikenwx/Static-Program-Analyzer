#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class UsesProcVarRelationship : public ProcVarRelationship {
 public:
  std::string procedureName() override;
  std::string variableName() override;
  static UsesProcVarRelationship CreateRelationship(
      std::string procedureName, std::string variableName);

 private:
  UsesProcVarRelationship(std::string procedureName, std::string variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
