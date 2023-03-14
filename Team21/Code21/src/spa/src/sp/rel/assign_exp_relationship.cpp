#include "assign_exp_relationship.h"

namespace rel {
std::unique_ptr<AssignExpRelationship> AssignExpRelationship::CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode, std::string postfixExp) {
  return std::unique_ptr<AssignExpRelationship>(new AssignExpRelationship(assignNode, postfixExp));
};

std::shared_ptr<ast::AssignNode> AssignExpRelationship::assignNode() const { return assignNode_; }

std::string AssignExpRelationship::varName() const { return assignNode_->GetVariable()->GetName(); }

int AssignExpRelationship::statementNumber() const { return assignNode_->GetStatementNumber(); }

std::string AssignExpRelationship::postfixExp() const { return postfixExp_; }

AssignExpRelationship::AssignExpRelationship(std::shared_ptr<ast::AssignNode> assignNode, std::string postfixExp) {
  assignNode_ = assignNode;
  postfixExp_ = postfixExp;
};
}
