#include "expression_subparser.h"
#include "sp/ast/additive_operation_node.h"
#include "sp/ast/astlib.h"
#include "util/instance_of.h"
#include "util/is_symbol_node_value.h"

namespace parser {
auto ExpressionSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol =
      [&](const std::shared_ptr<ast::INode> &node) {
        return IsSymbolNodeValueAnyOf(node, {"+", "-"});
      };
  if (context->IsLookaheadSymbolValueAnyOf(
          {")", ";", "+", "-", "<", ">", "==", "<=", ">=", "!="})) {
    // expr: expr ['+', '-'] term
    if (stack->size() >= 3 && util::instance_of<ast::TermNode>(*iter) &&
        is_correct_symbol(*std::next(iter, 1)) &&
        util::instance_of<ast::ExpressionNode>(*std::next(iter, 2))) {
      // References term node
      std::shared_ptr<ast::TermNode> ter =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // References additive symbol type
      std::string sym =
          std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops additive symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp1 =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates additive operation node
      std::shared_ptr<ast::AdditiveOperationNode> bin =
          std::make_shared<ast::AdditiveOperationNode>(exp1->GetOperand(),
                                                       ter->GetOperand(), sym);
      // Creates expression node
      std::shared_ptr<ast::ExpressionNode> exp2 =
          std::make_shared<ast::ExpressionNode>(bin);
      // Pushes expression node to parse stack
      stack->push_back(exp2);
      return true;
    }
    // expr: term
    if (util::instance_of<ast::TermNode>(*iter)) {
      // References term node
      std::shared_ptr<ast::TermNode> ter =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::make_shared<ast::ExpressionNode>(ter->GetOperand());
      // Pushes expression node to parse stack
      stack->push_back(exp);
      return true;
    }
  }
  return Subparser::Parse(context);
}
} // namespace parser
