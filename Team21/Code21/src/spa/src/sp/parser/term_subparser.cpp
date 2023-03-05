#include "sp/ast/astlib.h"
#include "term_subparser.h"
#include "token/right_paren_token.h"
#include "token/semicolon_token.h"
#include "token/plus_token.h"
#include "token/minus_token.h"
#include "token/multiply_token.h"
#include "token/divide_token.h"
#include "token/modulo_token.h"
#include "token/less_than_token.h"
#include "token/greater_than_token.h"
#include "token/equal_token.h"
#include "token/less_equal_token.h"
#include "token/greater_equal_token.h"
#include "token/not_equal_token.h"
#include "util/instance_of.h"

namespace parser {
bool TermSubparser::Parse(std::shared_ptr<Context> context) {
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
    if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMultiply
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T * F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::TermNode> t1 = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::TimesNode> b = std::make_shared<ast::TimesNode>(t1->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack->push_back(t2);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kDivide
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T / F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::DivideNode> b = std::make_shared<ast::DivideNode>(t->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack->push_back(t2);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kModulo
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      //T <- T % F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::ModuloNode> b = std::make_shared<ast::ModuloNode>(t->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack->push_back(t2);
      return true;
    } else if (util::instance_of<ast::FactorNode>(*i)) {
      // T <- F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::TermNode> t = std::make_shared<ast::TermNode>(f->GetOperand());
      stack->push_back(t);
      return true;
    }
  } else {
    return Subparser::Parse(context);
  }
}
}
