#pragma once

#include <string>

#include "relationship.h"

namespace rel{
class ProcRelationship : public Relationship {
 public:
  std::string procedureName();
  static ProcRelationship* CreateRelationship(std::string procedureName);

 private:
  ProcRelationship(std::string procedureName);
  std::string procedureName_;
};
} // namespace rel
