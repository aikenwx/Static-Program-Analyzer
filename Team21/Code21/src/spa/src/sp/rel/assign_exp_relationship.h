#pragma once

#include <string>
#include <utility>

#include "../ast/assign_node.h"
#include "relationship.h"

namespace rel {
class AssignExpRelationship : public Relationship {
 public:
  static auto CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode, const std::string& postfixExp) -> std::unique_ptr<AssignExpRelationship>;
  [[nodiscard]] auto assignNode() const -> std::shared_ptr<ast::AssignNode>;
  [[nodiscard]] auto varName() const -> std::string;
  [[nodiscard]] auto statementNumber() const -> int;
  [[nodiscard]] auto postfixExp() const -> std::string;
  [[nodiscard]] auto relationshipType() const -> RelationshipType override { return RelationshipType::PROC; };

 private:
  AssignExpRelationship(std::shared_ptr<ast::AssignNode> assignNode, std::string_view postfixExp) : assignNode_(std::move(assignNode)), postfixExp_(postfixExp) {};
  std::shared_ptr<ast::AssignNode> assignNode_;
  std::string postfixExp_;
};
} // namespace rel
