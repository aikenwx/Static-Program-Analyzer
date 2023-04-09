#include "sp/ast/astlib.h"
#include "conditional_expression_subparser.h"
#include "sp/ast/conditional_expression_node.h"
#include "sp/ast/logical_operation_node.h"
#include "util/instance_of.h"
#include "util/is_symbol_node_value.h"

namespace parser {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto ConditionalExpressionSubparser::Parse(std::shared_ptr<Context> context)
    -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol_logical = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return IsSymbolNodeValue(symbol_node, "&&") || IsSymbolNodeValue(symbol_node, "||");
  };
  auto is_correct_symbol_comparison = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return IsSymbolNodeValue(symbol_node, "==") || IsSymbolNodeValue(symbol_node, "!=")
      || IsSymbolNodeValue(symbol_node, "<") || IsSymbolNodeValue(symbol_node, ">")
      || IsSymbolNodeValue(symbol_node, "<=") || IsSymbolNodeValue(symbol_node, ">=");
  };
  auto is_relational_factor = [&](const std::shared_ptr<ast::INode> &node) {
    return util::instance_of<ast::ExpressionNode>(node)
      || util::instance_of<ast::IdentifierNode>(node)
      || util::instance_of<ast::ConstantNode>(node);
  };
  auto get_relational_factor = [&](const std::shared_ptr<ast::INode> &node) {
    if (util::instance_of<ast::ExpressionNode>(node)) {
      return std::static_pointer_cast<ast::ExpressionNode>(node)->GetOperand();
    }
    return node;
  };
  if (context->IsLookaheadSymbolValue(")")) {
    // cond_expr: '!' '(' cond_expr ')'
    if (stack->size() >= 4
      && IsSymbolNodeValue(*iter, ")")
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 1))
      && IsSymbolNodeValue(*std::next(iter, 2), "(")
      && IsSymbolNodeValue(*std::next(iter, 3), "!")) {
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp1 =
          std::static_pointer_cast<ast::ConditionalExpressionNode>(
              stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Pops not symbol node
      stack->pop_back();
      // Creates not node
      std::shared_ptr<ast::NotNode> uni =
          std::make_shared<ast::NotNode>(exp1->GetOperand());
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp2 =
          std::make_shared<ast::ConditionalExpressionNode>(uni);
      // Pushes conditional expression node to parse stack
      stack->push_back(exp2);
      return true;
    }
// cond_expr: '(' cond_expr ')' ['&&', '||] '(' cond_expr ')'
    if (stack->size() >= 7
      && IsSymbolNodeValue(*iter, ")")
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 1))
      && IsSymbolNodeValue(*std::next(iter, 2), "(")
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 3)) && is_correct_symbol_logical(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 3)))
      && IsSymbolNodeValue(*std::next(iter, 4), ")")
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 5))
      && IsSymbolNodeValue(*std::next(iter, 6), "(")) {
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp1 =
          std::static_pointer_cast<ast::ConditionalExpressionNode>(
              stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // References logical symbol type
      std::string sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops logical symbol node
      stack->pop_back();
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp2 =
          std::static_pointer_cast<ast::ConditionalExpressionNode>(
              stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Creates logical (binary) operation node
      std::shared_ptr<ast::LogicalOperationNode> bin =
          std::make_shared<ast::LogicalOperationNode>(exp1->GetOperand(), exp2->GetOperand(), sym);
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp3 =
          std::make_shared<ast::ConditionalExpressionNode>(bin);
      // Pushes conditional expression node to parse stack
      stack->push_back(exp3);
      return true;
    }
    // cond_expr: rel_expr
    // rel_expr: rel_factor ['==', '<', '>', '!=', '<=', '>='] rel_factor
    if (stack->size() >= 3
      && is_relational_factor(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && is_correct_symbol_comparison(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))
      && is_relational_factor(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::INode> fac1 = get_relational_factor(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // References comparison symbol type
      std::string sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops comparison symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::INode> fac2 = get_relational_factor(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates comparison operation node
      std::shared_ptr<ast::ComparisonOperationNode> bin =
          std::make_shared<ast::ComparisonOperationNode>(fac2, fac1, sym);
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp =
          std::make_shared<ast::ConditionalExpressionNode>(bin);
      // Pushes conditional expression node to parse stack
      stack->push_back(exp);
      return true;
    }
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
