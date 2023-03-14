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
  // Default parsing case (LHS of relational expression) 
  if (context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    // L <- E
    if (util::instance_of<ast::ExpressionNode>(*i)) {
      // References expression node
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(e->GetOperand());
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
    // L <- V
    if (util::instance_of<ast::VariableNode>(*i)) {
      // References variable node
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(v);
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
    // L <- C
    if (util::instance_of<ast::ConstantNode>(*i)) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pops constant node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(c);
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
  }
  // Secondary parsing case (RHS of relational expression)
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // L <- E
    if (stack->size() >= 2
      && util::instance_of<ast::ExpressionNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // References expression node
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(e->GetOperand());
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
    // L <- V
    if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // References variable node
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(v);
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
    // L <- C
    if (stack->size() >= 2
      && util::instance_of<ast::ConstantNode>(*i)
      && (util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual)) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pop constant node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f = std::make_shared<ast::RelationalFactorNode>(c);
      // Pushes relational factor node to parse stack
      stack->push_back(f);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
