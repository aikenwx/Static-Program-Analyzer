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
bool ExpressionSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
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
    if (stack->size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kPlus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E + T <;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::PlusNode> b = std::make_shared<ast::PlusNode>(e1->GetOperand(), t->GetOperand());
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      stack->push_back(e2);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMinus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E - T <;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::MinusNode> b = std::make_shared<ast::MinusNode>(e1->GetOperand(), t->GetOperand());
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      stack->push_back(e2);
      return true;
    } else if (util::instance_of<ast::TermNode>(*i)) {
      // E <- T < ;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::ExpressionNode> e = std::make_shared<ast::ExpressionNode>(t->GetOperand());
      stack->push_back(e);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
