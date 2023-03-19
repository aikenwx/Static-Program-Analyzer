#include "sp/ast/astlib.h"
#include "factor_subparser.h"
#include "sp/token/right_paren_token.h"
#include "sp/token/semicolon_token.h"
#include "sp/token/plus_token.h"
#include "sp/token/minus_token.h"
#include "sp/token/multiply_token.h"
#include "sp/token/divide_token.h"
#include "sp/token/modulo_token.h"
#include "sp/token/less_than_token.h"
#include "sp/token/greater_than_token.h"
#include "sp/token/equal_token.h"
#include "sp/token/less_equal_token.h"
#include "sp/token/greater_equal_token.h"
#include "sp/token/not_equal_token.h"
#include "util/instance_of.h"

namespace parser {
auto FactorSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()
    || context->IsLookaheadTypeOf<token::SemicolonToken>()
    || context->IsLookaheadTypeOf<token::PlusToken>()
    || context->IsLookaheadTypeOf<token::MinusToken>()
    || context->IsLookaheadTypeOf<token::MultiplyToken>()
    || context->IsLookaheadTypeOf<token::DivideToken>()
    || context->IsLookaheadTypeOf<token::ModuloToken>()
    || context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    // factor: var_name
    if (util::instance_of<ast::VariableNode>(*iter)) {
      // References variable node
      std::shared_ptr<ast::VariableNode> var =
          std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates factor node
      std::shared_ptr<ast::FactorNode> fac =
          std::make_shared<ast::FactorNode>(var);
      // Pushes factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // factor: const_value
    if (util::instance_of<ast::ConstantNode>(*iter)) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> con =
          std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pops constant node
      stack->pop_back();
      // Creates factor node
      std::shared_ptr<ast::FactorNode> fac =
          std::make_shared<ast::FactorNode>(con);
      // Pushes factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // factor: '(' expr ')'
    if (stack->size() >= 3
      && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ExpressionNode>(*std::next(iter, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 2)))->GetType() == ast::SymbolType::kLeftParen) {
      // Pops right paren symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Creates factor node
      std::shared_ptr<ast::FactorNode> fac =
          std::make_shared<ast::FactorNode>(exp->GetOperand());
      // Pushes factor node to parse stack
      stack->push_back(fac);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
