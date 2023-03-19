#include "sp/ast/astlib.h"
#include "relational_expression_subparser.h"
#include "sp/token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto RelationalExpressionSubparser::Parse(std::shared_ptr<Context> context)
    -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // rel_expr: rel_factor '>' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kGreater
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops greater symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates greater node
      std::shared_ptr<ast::GreaterNode> bin =
          std::make_shared<ast::GreaterNode>(fac2->GetOperand(),
                                             fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
    // rel_expr: rel_factor '<' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kLesser
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops lesser symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates lesser node
      std::shared_ptr<ast::LesserNode> bin =
          std::make_shared<ast::LesserNode>(fac2->GetOperand(), fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
    // rel_expr: rel_factor '==' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates equals node
      std::shared_ptr<ast::EqualsNode> bin =
          std::make_shared<ast::EqualsNode>(fac2->GetOperand(), fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
    // rel_expr: rel_factor '>=' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kGreaterEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops greater equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates greater equals node
      std::shared_ptr<ast::GreaterEqualsNode> bin =
          std::make_shared<ast::GreaterEqualsNode>(fac2->GetOperand(),
                                                   fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
    // rel_expr: rel_factor '<=' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kLesserEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops lesser equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates lesser equals node
      std::shared_ptr<ast::LesserEqualsNode> bin =
          std::make_shared<ast::LesserEqualsNode>(fac2->GetOperand(),
                                                  fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
    // rel_expr: rel_factor '!=' rel_factor
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*iter)
      && util::instance_of<ast::SymbolNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 1)))->GetType() == ast::SymbolType::kNotEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(iter, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac1 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops not equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> fac2 =
          std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates not equals node
      std::shared_ptr<ast::NotEqualsNode> bin =
          std::make_shared<ast::NotEqualsNode>(fac2->GetOperand(),
                                               fac1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> exp =
          std::make_shared<ast::RelationalExpressionNode>(bin);
      // Pushes relational expression node to parse stack
      stack->push_back(exp);
      return true;
    }
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
