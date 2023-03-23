#include "sp/ast/astlib.h"
#include "conditional_expression_subparser.h"
#include "sp/ast/logical_operation_node.h"
#include "sp/token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto ConditionalExpressionSubparser::Parse(std::shared_ptr<Context> context)
    -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return symbol_node->GetType() == ast::SymbolType::kAnd
      || symbol_node->GetType() == ast::SymbolType::kOr;
  };
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // cond_expr: '!' '(' cond_expr ')'
    if (stack->size() >= 4
      && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 3)))->GetType() == ast::SymbolType::kNot) {
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
      && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 3)) && is_correct_symbol(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 3)))
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 4)))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 6)))->GetType() == ast::SymbolType::kLeftParen) {
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
      ast::SymbolType sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
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
    if (util::instance_of<ast::RelationalExpressionNode>(*iter)) {
      // References relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> rel =
          std::static_pointer_cast<ast::RelationalExpressionNode>(
              stack->back());
      // Pops relational expression node
      stack->pop_back();
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> exp =
          std::make_shared<ast::ConditionalExpressionNode>(rel->GetOperand());
      // Pushes conditional expression node to parse stack
      stack->push_back(exp);
      return true;
    }
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
