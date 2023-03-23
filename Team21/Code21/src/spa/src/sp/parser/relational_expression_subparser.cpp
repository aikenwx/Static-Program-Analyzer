#include "sp/ast/astlib.h"
#include "relational_expression_subparser.h"
#include "sp/ast/comparison_operation_node.h"
#include "sp/token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto RelationalExpressionSubparser::Parse(std::shared_ptr<Context> context)
    -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return symbol_node->GetType() == ast::SymbolType::kEqual
      || symbol_node->GetType() == ast::SymbolType::kLesser
      || symbol_node->GetType() == ast::SymbolType::kGreater
      || symbol_node->GetType() == ast::SymbolType::kNotEqual
      || symbol_node->GetType() == ast::SymbolType::kLesserEqual
      || symbol_node->GetType() == ast::SymbolType::kGreaterEqual;
  };
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // rel_expr: rel_factor ['==', '<', '>', '!=', '<=', '>='] rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && is_correct_symbol(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // References comparison symbol type
      ast::SymbolType sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops comparison symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates comparison operation node
      std::shared_ptr<ast::ComparisonOperationNode> bin =
          std::make_shared<ast::ComparisonOperationNode>(fac2->GetOperand(),
                                             fac1->GetOperand(),
                                             sym);
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
