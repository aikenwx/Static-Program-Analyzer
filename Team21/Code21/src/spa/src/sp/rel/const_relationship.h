#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ConstRelationship : public Relationship {
 public:
  int value();
  static ConstRelationship* CreateRelationship(int value);

 private:
  ConstRelationship(int value);
  int value_;
};
} // namespace rel
