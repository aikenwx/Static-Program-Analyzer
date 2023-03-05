#include "sp/ast/astlib.h"
#include "conditional_expression_subparser.h"
#include "token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
bool ConditionalExpressionSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    if (stack->size() >= 4
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kNot) {
      // O <- ! ( O )
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> n = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::NotNode> u = std::make_shared<ast::NotNode>(n->GetOperand());
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(u);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kAnd
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 4)))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 6)))->GetType() == ast::SymbolType::kLeftParen) {
      // O <- ( O ) && ( O )
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> n1 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> n2 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::AndNode> b = std::make_shared<ast::AndNode>(n2->GetOperand(), n1->GetOperand());
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kOr
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 4)))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 6)))->GetType() == ast::SymbolType::kLeftParen) {
      // O <- ( O ) || ( O )
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> n1 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> n2 = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::OrNode> b = std::make_shared<ast::OrNode>(n2->GetOperand(), n1->GetOperand());
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (util::instance_of<ast::RelationalExpressionNode>(*i)) {
      // O <- R
      std::shared_ptr<ast::RelationalExpressionNode> r = std::static_pointer_cast<ast::RelationalExpressionNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::ConditionalExpressionNode> e = std::make_shared<ast::ConditionalExpressionNode>(r->GetOperand());
      stack->push_back(e);
      return true;
    }
  } else {
    return Subparser::Parse(context);
  }
}
}
