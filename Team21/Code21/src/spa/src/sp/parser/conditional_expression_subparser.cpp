#include "sp/ast/astlib.h"
#include "conditional_expression_subparser.h"
#include "sp/token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
bool ConditionalExpressionSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    // O <- ! ( O )
    if (stack->size() >= 4
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kNot) {
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> n = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Pops not symbol node
      stack->pop_back();
      // Creates not node
      std::shared_ptr<ast::NotNode> u = std::make_shared<ast::NotNode>(n->GetOperand());
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(u);
      // Pushes conditional expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // O <- ( O ) && ( O )
    if (stack->size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kAnd
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 4)))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 6)))->GetType() == ast::SymbolType::kLeftParen) {
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> n1 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      // Pops conditionl expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Pops and symbol node
      stack->pop_back();
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> n2 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Creates and node
      std::shared_ptr<ast::AndNode> b = std::make_shared<ast::AndNode>(n2->GetOperand(), n1->GetOperand());
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(b);
      // Pushes conditional expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // O <- ( O ) || ( O )
    if (stack->size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kOr
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 4)))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 6)))->GetType() == ast::SymbolType::kLeftParen) {
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> n1 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Pops or symbol node
      stack->pop_back();
      // Pops right paren symbol node
      stack->pop_back();
      // References conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> n2 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      // Pops conditional expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Creates or node
      std::shared_ptr<ast::OrNode> b = std::make_shared<ast::OrNode>(n2->GetOperand(), n1->GetOperand());
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(b);
      // Pushes conditional expression node to parse stack
      stack->push_back(e);
      return true;
    }
    // O <- R
    if (util::instance_of<ast::RelationalExpressionNode>(*i)) {
      // References relational expression node
      std::shared_ptr<ast::RelationalExpressionNode> r = std::static_pointer_cast<ast::RelationalExpressionNode>(stack->back());
      // Pops relational expression node
      stack->pop_back();
      // Creates conditional expression node
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(r->GetOperand());
      // Pushes conditional expression node to parse stack
      stack->push_back(e);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
