#include "assign_exp_extractor.h"

#include <stack>

#include "../ast/binary_operation_node.h"
#include "../ast/plus_node.h"
#include "../ast/minus_node.h"
#include "../ast/times_node.h"
#include "../ast/divide_node.h"
#include "../ast/modulo_node.h"
#include "../ast/constant_node.h"
#include "../ast/variable_node.h"
#include "../rel/assign_exp_relationship.h"
#include "../rel/relationship.h"
#include "util/instance_of.h"

namespace design_extractor {
std::string BinExpExprNodeToOperator(std::shared_ptr<ast::BinaryOperationNode> node) {
  if (util::instance_of<ast::PlusNode>(node)) {
    return "+";
  } else if (util::instance_of<ast::MinusNode>(node)) {
    return "-";
  } else if (util::instance_of<ast::TimesNode>(node)) {
    return "*";
  } else if (util::instance_of<ast::DivideNode>(node)) {
    return "/";
  } else if (util::instance_of<ast::ModuloNode>(node)) {
    return "%";
  } else {
    return "";
  }
}

std::stack<std::string> AssignExpToPostfixExpStack(std::shared_ptr<ast::INode> node) {
  std::stack<std::string> postfixExpStack;

  if (util::instance_of<ast::ConstantNode>(node)) {
    std::shared_ptr<ast::ConstantNode> constantNode = std::static_pointer_cast<ast::ConstantNode>(node);
    // no std::format before c++20, unfortunate
    postfixExpStack.push('"' + std::to_string(constantNode->GetValue()) + '"');
  } else if (util::instance_of<ast::VariableNode>(node)) {
    std::shared_ptr<ast::VariableNode> variableNode = std::static_pointer_cast<ast::VariableNode>(node);
    postfixExpStack.push('"' + variableNode->GetName() + '"');
  } else if (util::instance_of<ast::BinaryOperationNode>(node)) {
    std::shared_ptr<ast::BinaryOperationNode> binaryOperationNode = std::static_pointer_cast<ast::BinaryOperationNode>(node);
    std::stack<std::string> leftExpStack = AssignExpToPostfixExpStack(binaryOperationNode->GetLeft());
    std::stack<std::string> rightExpStack = AssignExpToPostfixExpStack(binaryOperationNode->GetRight());

    while (!leftExpStack.empty()) {
      postfixExpStack.push(leftExpStack.top());
      leftExpStack.pop();
    }
    while (!rightExpStack.empty()) {
      postfixExpStack.push(rightExpStack.top());
      rightExpStack.pop();
    }

    postfixExpStack.push(BinExpExprNodeToOperator(binaryOperationNode));
  }
  return postfixExpStack;
}

std::string AssignExpToPostfixExp(std::shared_ptr<ast::INode> node) {
  std::stack<std::string> postfixExpStack = AssignExpToPostfixExpStack(node);
  std::string postfixExp = "";

  while (!postfixExpStack.empty()) {
    postfixExp += (postfixExpStack.top() + " ");
    postfixExpStack.pop();
  }

  return postfixExp;
}

std::optional<std::vector<std::unique_ptr<rel::Relationship>>>
AssignExpExtractor::HandleAssignNode(
    std::vector<std::shared_ptr<ast::INode>> parents,
    std::shared_ptr<ast::AssignNode> node) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  std::string postfixExp = AssignExpToPostfixExp(node->GetAssignment());
  std::unique_ptr<rel::Relationship> relationship = rel::AssignExpRelationship::CreateRelationship(node, postfixExp);
  relationships.push_back(std::move(relationship));

  return relationships;
}
}  // namespace design_extractor
