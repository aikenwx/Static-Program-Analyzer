#include "sp/ast/astlib.h"
#include "relational_factor_subparser.h"
#include "sp/token/right_paren_token.h"
#include "sp/token/less_than_token.h"
#include "sp/token/greater_than_token.h"
#include "sp/token/equal_token.h"
#include "sp/token/less_equal_token.h"
#include "sp/token/greater_equal_token.h"
#include "sp/token/not_equal_token.h"
#include "util/instance_of.h"

namespace parser {
bool RelationalFactorSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    if (util::instance_of<ast::ExpressionNode>(*i)) {
      // L <- E
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(e->GetOperand());
      stack->push_back(f);
      return true;
    } else if (util::instance_of<ast::VariableNode>(*i)) {
      // L <- V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(v);
      stack->push_back(f);
      return true;
    } else if (util::instance_of<ast::ConstantNode>(*i)) {
      // L <- C
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(c);
      stack->push_back(f);
      return true;
    }
  } else if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    if (stack->size() >= 2
      && util::instance_of<ast::ExpressionNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // L <- E
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(e->GetOperand());
      stack->push_back(f);
      return true;
    } else if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // L <- V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(v);
      stack->push_back(f);
      return true;
    } else if (stack->size() >= 2
      && util::instance_of<ast::ConstantNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // L <- C
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(c);
      stack->push_back(f);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
