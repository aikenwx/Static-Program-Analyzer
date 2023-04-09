#include "assign_exp_relationship.h"

#include <utility>

namespace rel {
auto AssignExpRelationship::CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode, const std::string& postfixExp) -> std::unique_ptr<AssignExpRelationship> {
  return std::unique_ptr<AssignExpRelationship>(new AssignExpRelationship(std::move(assignNode), postfixExp));
};

auto AssignExpRelationship::assignNode() const -> std::shared_ptr<ast::AssignNode> { return assignNode_; }

auto AssignExpRelationship::varName() const -> std::string { return assignNode_->GetVariable()->GetValue(); }

auto AssignExpRelationship::statementNumber() const -> int { return assignNode_->GetStatementNumber(); }

auto AssignExpRelationship::postfixExp() const -> std::string { return postfixExp_; }
}  // namespace rel
