#include "sp/ast/astlib.h"
#include "relational_expression_subparser.h"
#include "sp/token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
bool RelationalExpressionSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // R <- L > L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops greater symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates greater node
      std::shared_ptr<ast::GreaterNode> b = std::make_shared<ast::GreaterNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // R <- L < L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops lesser symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates lesser node
      std::shared_ptr<ast::LesserNode> b = std::make_shared<ast::LesserNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // R <- L == L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates equals node
      std::shared_ptr<ast::EqualsNode> b = std::make_shared<ast::EqualsNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // R <- L >= L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops greater equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates greater equals node
      std::shared_ptr<ast::GreaterEqualsNode> b = std::make_shared<ast::GreaterEqualsNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // R <- L <= L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops lesser equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates lesser equals node
      std::shared_ptr<ast::LesserEqualsNode> b = std::make_shared<ast::LesserEqualsNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // R <- L == L
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Pops not equals symbol node
      stack->pop_back();
      // References relational factor node
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      // Pops relational factor node
      stack->pop_back();
      // Creates not equals node
      std::shared_ptr<ast::NotEqualsNode> b = std::make_shared<ast::NotEqualsNode>(f2->GetOperand(), f1->GetOperand());
      // Creates relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      // Pushes relational expression node to parse stack
      stack->push_back(e);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
