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
auto RelationalFactorSubparser::Parse(std::shared_ptr<Context> context)
    -> bool {
  if (ParsePrimaryCase(context)) {
    return true;
  }
  if (ParseSecondaryCase(context)) {
    return true;
  }
  return Subparser::Parse(context);
}

auto RelationalFactorSubparser::ParsePrimaryCase(const std::shared_ptr<Context> &context)
    -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // Default parsing case (LHS of relational expression)
  if (context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    // rel_factor: expr
    if (util::instance_of<ast::ExpressionNode>(*iter)) {
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(exp->GetOperand());
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // rel_factor: var_name
    if (util::instance_of<ast::VariableNode>(*iter)) {
      // References variable node
      std::shared_ptr<ast::VariableNode> var =
          std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(var);
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // rel_factor: const_value
    if (util::instance_of<ast::ConstantNode>(*iter)) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> con =
          std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pops constant node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(con);
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
  }
  return false;
}

auto RelationalFactorSubparser::ParseSecondaryCase(const std::shared_ptr<Context> &context)
  -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // Secondary parsing case (RHS of relational expression)
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // Checks validity of conditions for rel_factor to be parsed
    bool is_valid = util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kLesser
        || util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kGreater
        || util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kEqual
        || util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kLesserEqual
        || util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kGreaterEqual
        || util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kNotEqual;
    // rel_factor: expr
    if (stack->size() >= 2
      && util::instance_of<ast::ExpressionNode>(*iter)
      && is_valid) {
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(exp->GetOperand());
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // rel_factor: var_name
    if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*iter)
      && is_valid) {
      // References variable node
      std::shared_ptr<ast::VariableNode> var =
          std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(var);
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
    // rel_factor: const_value
    if (stack->size() >= 2
      && util::instance_of<ast::ConstantNode>(*iter)
      && is_valid) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> con =
          std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pop constant node
      stack->pop_back();
      // Creates relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac =
          std::make_shared<ast::RelationalFactorNode>(con);
      // Pushes relational factor node to parse stack
      stack->push_back(fac);
      return true;
    }
  }
  return false;
}
}  // namespace parser
