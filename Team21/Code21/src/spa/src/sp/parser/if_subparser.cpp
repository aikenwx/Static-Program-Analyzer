#include "sp/ast/astlib.h"
#include "if_subparser.h"
#include "util/instance_of.h"

namespace parser {
bool IfSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  // Could add token checks in the future to guarantee more correctness
  if (stack->size() >= 7
    && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 4))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 5)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 5)))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NamedNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::NamedNode>(*std::next(i, 6)))->GetName() == "while") {
    // Sc(w) <- while ( O ) { S+ }
    // Currently a named node instead of identifier node
    stack->pop_back();
    std::shared_ptr<ast::StatementListNode> s = std::static_pointer_cast<ast::StatementListNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::ConditionalExpressionNode> e = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::WhileNode> w = std::make_shared<ast::WhileNode>(e->GetOperand(), s);
    context->GetStatementCounter()++;
    stack->push_back(w);
    return true;
  } else {
    return Subparser::Parse(context);
  }
}
}
