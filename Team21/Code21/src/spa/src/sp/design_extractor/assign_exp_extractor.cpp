#include "assign_exp_extractor.h"

#include <memory>
#include <queue>
#include <sstream>

#include "../ast/astlib.h"
#include "../rel/assign_exp_relationship.h"
#include "../rel/relationship.h"
#include "sp/ast/additive_operation_node.h"
#include "sp/ast/multiplicative_operation_node.h"
#include "util/instance_of.h"

namespace design_extractor {
auto BinExpExprNodeToOperator(
    const std::shared_ptr<ast::BinaryOperationNode> &node) -> std::string {
  if (util::instance_of<ast::AdditiveOperationNode>(node)) {
    return std::static_pointer_cast<ast::AdditiveOperationNode>(node)->GetSymbolType();
  }
  if (util::instance_of<ast::MultiplicativeOperationNode>(node)) {
    return std::static_pointer_cast<ast::MultiplicativeOperationNode>(node)->GetSymbolType();
  }
  return "";
}

auto AssignExpToPostfixExpStack(const std::shared_ptr<ast::INode> &node)
    -> std::queue<std::string> {
  std::queue<std::string> postfixExpStack;

  if (util::instance_of<ast::ConstantNode>(node)) {
    std::shared_ptr<ast::ConstantNode> constantNode =
        std::static_pointer_cast<ast::ConstantNode>(node);
    // no std::format before c++20, unfortunate
    postfixExpStack.push('"' + std::to_string(constantNode->GetValue()) + '"');
  } else if (util::instance_of<ast::VariableNode>(node)) {
    std::shared_ptr<ast::VariableNode> variableNode =
        std::static_pointer_cast<ast::VariableNode>(node);
    postfixExpStack.push('"' + variableNode->GetName() + '"');
  } else if (util::instance_of<ast::BinaryOperationNode>(node)) {
    std::shared_ptr<ast::BinaryOperationNode> binaryOperationNode =
        std::static_pointer_cast<ast::BinaryOperationNode>(node);
    std::queue<std::string> leftExpStack =
        AssignExpToPostfixExpStack(binaryOperationNode->GetLeft());
    std::queue<std::string> rightExpStack =
        AssignExpToPostfixExpStack(binaryOperationNode->GetRight());

    while (!leftExpStack.empty()) {
      postfixExpStack.push(leftExpStack.front());
      leftExpStack.pop();
    }
    while (!rightExpStack.empty()) {
      postfixExpStack.push(rightExpStack.front());
      rightExpStack.pop();
    }

    postfixExpStack.push(BinExpExprNodeToOperator(binaryOperationNode));
  }
  return postfixExpStack;
}

auto AssignExpToPostfixExp(const std::shared_ptr<ast::INode>& node) -> std::string {
  std::queue<std::string> postfixExpStack = AssignExpToPostfixExpStack(node);
  std::string postfixExp;

  std::ostringstream pfeStream;

  while (!postfixExpStack.empty()) {
    pfeStream << postfixExpStack.front();
    postfixExpStack.pop();
  }

  return pfeStream.str();
}

void AssignExpExtractor::HandleAssignNode(
    const std::shared_ptr<ast::AssignNode> &node, int depth) {
  std::string postfixExp = AssignExpToPostfixExp(node->GetAssignment());
  std::shared_ptr<rel::AssignExpRelationship> relationship =
      rel::AssignExpRelationship::CreateRelationship(node, postfixExp);
  relns_.push_back(relationship);
}

auto
AssignExpExtractor::GetRelationships() const -> std::vector<std::shared_ptr<rel::AssignExpRelationship>> {
  return relns_;
}
} // namespace design_extractor
