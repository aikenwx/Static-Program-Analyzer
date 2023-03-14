#include "sp/ast/astlib.h"
#include "term_subparser.h"
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
    // T <- T * F
    if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMultiply
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // References factor node
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // Pops times symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> t1 = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates times node
      std::shared_ptr<ast::TimesNode> b = std::make_shared<ast::TimesNode>(t1->GetOperand(), f->GetOperand());
      // Creates term node
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      // Pushes term node to parse stack
      stack->push_back(t2);
      return true;
    }
    // T <- T / F
    if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kDivide
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // References factor node
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // Pops divides symbol node
      stack->pop_back();
      // References term noe
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates divide node
      std::shared_ptr<ast::DivideNode> b = std::make_shared<ast::DivideNode>(t->GetOperand(), f->GetOperand());
      // Creates term node
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      // Pushes term node to parse stack
      stack->push_back(t2);
      return true;
    }
    // T <- T % F
    if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kModulo
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // References factor node
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // Pops modulo symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates modulo node
      std::shared_ptr<ast::ModuloNode> b = std::make_shared<ast::ModuloNode>(t->GetOperand(), f->GetOperand());
      // Creates term node
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      // Pushes term node to parse stack
      stack->push_back(t2);
      return true;
    }
    // T <- F
    if (util::instance_of<ast::FactorNode>(*i)) {
      // References factor node
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // Creates term node
      std::shared_ptr<ast::TermNode> t = std::make_shared<ast::TermNode>(f->GetOperand());
      // Pushes term node to parse stack
      stack->push_back(t);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
