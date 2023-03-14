#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ProcRelationship : public Relationship {
 public:
  static std::unique_ptr<ProcRelationship> CreateRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode);
  std::shared_ptr<ast::ProcedureNode> procedureNode() const;
  std::string procedureName() const;
  RelationshipType relationshipType() const override { return RelationshipType::PROC; };

 private:
  ProcRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode);
  std::shared_ptr<ast::ProcedureNode> procedureNode_;
};
} // namespace rel
