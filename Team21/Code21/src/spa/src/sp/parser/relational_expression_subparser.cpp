#include "sp/ast/astlib.h"
#include "relational_expression_subparser.h"
#include "token/right_paren_token.h"
#include "util/instance_of.h"

namespace parser {
bool RelationalExpressionSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightParenToken>()) {
    if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreater
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L > L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::GreaterNode> b = std::make_shared<ast::GreaterNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesser
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L < L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::LesserNode> b = std::make_shared<ast::LesserNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L == L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::EqualsNode> b = std::make_shared<ast::EqualsNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kGreaterEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L >= L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::GreaterEqualsNode> b = std::make_shared<ast::GreaterEqualsNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kLesserEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L <= L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::LesserEqualsNode> b = std::make_shared<ast::LesserEqualsNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    } else if (stack->size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kNotEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L == L
      std::shared_ptr<ast::RelationalFactorNode> f1 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::RelationalFactorNode> f2 = std::static_pointer_cast<ast::RelationalFactorNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::NotEqualsNode> b = std::make_shared<ast::NotEqualsNode>(f2->GetOperand(), f1->GetOperand());
      std::shared_ptr<ast::RelationalExpressionNode> e = std::make_shared<ast::RelationalExpressionNode>(b);
      stack->push_back(e);
      return true;
    }
  } else {
    return Subparser::Parse(context);
  }
}
}
