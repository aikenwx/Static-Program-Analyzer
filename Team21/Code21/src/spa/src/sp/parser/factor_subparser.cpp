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
bool FactorSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
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
    if (util::instance_of<ast::VariableNode>(*i)) {
      // F <- V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(v);
      stack->push_back(f);
      return true;
    } else if (util::instance_of<ast::ConstantNode>(*i)) {
      // F <- C
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(c);
      stack->push_back(f);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen) {
      // F <- ( E )
      stack->pop_back();
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(e->GetOperand());
      stack->push_back(f);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
