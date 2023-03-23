#include "sp/ast/astlib.h"
#include "sp/ast/multiplicative_operation_node.h"
#include "sp/ast/symbol_node.h"
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
auto TermSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol = [&](const std::shared_ptr<ast::SymbolNode> &symbol_node) {
    return symbol_node->GetType() == ast::SymbolType::kMultiply
      || symbol_node->GetType() == ast::SymbolType::kDivide
      || symbol_node->GetType() == ast::SymbolType::kModulo;
  };
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
    // term: term '*', '/', '%' factor
    if (stack->size() >= 3
      && util::instance_of<ast::FactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && is_correct_symbol(std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))
      && util::instance_of<ast::TermNode>(*std::next(iter, 2))) {
      // References factor node
      std::shared_ptr<ast::FactorNode> fac =
          std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // References symbol type
      ast::SymbolType sym = std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> ter1 =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates multiplicative operation node
      std::shared_ptr<ast::MultiplicativeOperationNode> bin =
          std::make_shared<ast::MultiplicativeOperationNode>(ter1->GetOperand(), fac->GetOperand(), sym);
      // Creates term node
      std::shared_ptr<ast::TermNode> ter2 =
          std::make_shared<ast::TermNode>(bin);
      // Pushes term node to parse stack
      stack->push_back(ter2);
      return true;
    }
    // term: factor
    if (util::instance_of<ast::FactorNode>(*iter)) {
      // References factor node
      std::shared_ptr<ast::FactorNode> fac =
          std::static_pointer_cast<ast::FactorNode>(stack->back());
      // Pops factor node
      stack->pop_back();
      // Creates term node
      std::shared_ptr<ast::TermNode> ter =
          std::make_shared<ast::TermNode>(fac->GetOperand());
      // Pushes term node to parse stack
      stack->push_back(ter);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
