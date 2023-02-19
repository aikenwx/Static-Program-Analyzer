#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ProcRelationship : public Relationship {
 public:
  static std::unique_ptr<ProcRelationship> CreateRelationship(std::string procedureName);
  std::string procedureName();
  RelationshipType relationshipType() override { return RelationshipType::PROC; };

 private:
  ProcRelationship(std::string procedureName);
  std::string procedureName_;
};
} // namespace rel
