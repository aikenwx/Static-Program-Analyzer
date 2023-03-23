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
    // rel_factor: iden
    if (util::instance_of<ast::IdentifierNode>(*iter)) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
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
    // First checks stack size and presence of symbol node
    if (stack->size() < 2 ||
        !util::instance_of<ast::SymbolNode>(*std::next(iter, 1))) {
      return false;
    }
    // Then checks type of symbol node
    if (auto nodeType =
            std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1))
                ->GetType();
        nodeType != ast::SymbolType::kLesser &&
        nodeType != ast::SymbolType::kGreater &&
        nodeType != ast::SymbolType::kEqual &&
        nodeType != ast::SymbolType::kLesserEqual &&
        nodeType != ast::SymbolType::kGreaterEqual &&
        nodeType != ast::SymbolType::kNotEqual) {
      return false;
    }
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
    // rel_factor: iden
    if (util::instance_of<ast::IdentifierNode>(*iter)) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
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
