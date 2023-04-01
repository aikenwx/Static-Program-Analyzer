#include "sp/ast/additive_operation_node.h"
#include "sp/ast/astlib.h"
#include "expression_subparser.h"
#include "sp/token/right_paren_token.h"
#include "sp/token/semicolon_token.h"
#include "sp/token/plus_token.h"
#include "sp/token/minus_token.h"
#include "sp/token/less_than_token.h"
#include "sp/token/greater_than_token.h"
#include "sp/token/equal_token.h"
#include "sp/token/less_equal_token.h"
#include "sp/token/greater_equal_token.h"
#include "sp/token/not_equal_token.h"
#include "util/instance_of.h"

namespace parser {
auto ExpressionSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return symbol_node->GetType() == ast::SymbolType::kPlus
      || symbol_node->GetType() == ast::SymbolType::kMinus;
  };
  if (context->IsLookaheadTypeOf<token::RightParenToken>()
    || context->IsLookaheadTypeOf<token::SemicolonToken>()
    || context->IsLookaheadTypeOf<token::PlusToken>()
    || context->IsLookaheadTypeOf<token::MinusToken>()
    || context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    // expr: expr ['+', '-'] term
    if (stack->size() >= 3
      && util::instance_of<ast::TermNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && is_correct_symbol(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))
      && util::instance_of<ast::ExpressionNode>(*std::next(iter, 2))) {
      // References term node
      std::shared_ptr<ast::TermNode> ter =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // References additive symbol type
      ast::SymbolType sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops additive symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp1 =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates additive operation node
      std::shared_ptr<ast::AdditiveOperationNode> bin =
          std::make_shared<ast::AdditiveOperationNode>(exp1->GetOperand(), ter->GetOperand(), sym);
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
}  // namespace parser
