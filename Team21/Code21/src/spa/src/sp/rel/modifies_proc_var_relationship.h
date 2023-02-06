#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ModifiesProcVarRelationship : public ProcVarRelationship {
 public:
  std::string procedureName() override;
  std::string variableName() override;
  static ModifiesProcVarRelationship CreateRelationship(
      std::string procedureName, std::string variableName);

 private:
  ModifiesProcVarRelationship(std::string procedureName,
                              std::string variableName);
  std::string procedureName_;
  std::string variableName_;
};
}  // namespace rel
