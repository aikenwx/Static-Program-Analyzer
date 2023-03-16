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
    // expr: expr '+' term
    if (stack->size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kPlus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // References term node
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Pops plus symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates plus node
      std::shared_ptr<ast::PlusNode> b = std::make_shared<ast::PlusNode>(e1->GetOperand(), t->GetOperand());
      // Creates expression node
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      // Pushes expression node to parse stack
      stack->push_back(e2);
      return true;
    }
    // expr: expr '-' term
    if (stack->size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMinus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // References term node
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Pops minus symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates minus node
      std::shared_ptr<ast::MinusNode> b = std::make_shared<ast::MinusNode>(e1->GetOperand(), t->GetOperand());
      // Creates expression node
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      // Pushes expression node to parse stack
      stack->push_back(e2);
      return true;
    }
    // expr: term
    if (util::instance_of<ast::TermNode>(*i)) {
      // References term node
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates expression node
      std::shared_ptr<ast::ExpressionNode> e = std::make_shared<ast::ExpressionNode>(t->GetOperand());
      // Pushes expression node to parse stack
      stack->push_back(e);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
