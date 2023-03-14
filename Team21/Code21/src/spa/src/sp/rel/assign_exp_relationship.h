#pragma once

#include <string>
#include <utility>

#include "../ast/assign_node.h"
#include "relationship.h"

namespace rel {
class AssignExpRelationship : public Relationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode, const std::string& postfixExp) -> std::unique_ptr<AssignExpRelationship>;
  auto assignNode() const -> std::shared_ptr<ast::AssignNode>;
  auto varName() const -> std::string;
  auto statementNumber() const -> int;
  auto postfixExp() const -> std::string;
  auto relationshipType() const -> RelationshipType override { return RelationshipType::PROC; };

 private:
  AssignExpRelationship(std::shared_ptr<ast::AssignNode> assignNode, std::string_view postfixExp) : assignNode_(std::move(assignNode)), postfixExp_(postfixExp) {};
  std::shared_ptr<ast::AssignNode> assignNode_;
  std::string postfixExp_;
};
} // namespace rel
