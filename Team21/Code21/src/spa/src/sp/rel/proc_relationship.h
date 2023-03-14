#pragma once

#include <string>

#include "relationship.h"

namespace rel {
class ProcRelationship : public Relationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<ProcRelationship>;
  auto procedureNode() const -> std::shared_ptr<ast::ProcedureNode>;
  auto procedureName() const -> std::string;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::PROC; };

 private:
  explicit ProcRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) : procedureNode_(std::move(procedureNode)) {};
  std::shared_ptr<ast::ProcedureNode> procedureNode_;
};
} // namespace rel
