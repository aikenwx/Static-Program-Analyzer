#include "sp/ast/astlib.h"
#include "while_subparser.h"
#include "util/instance_of.h"

namespace parser {
bool WhileSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (stack->size() >= 12
    && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NamedNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::NamedNode>(*std::next(i, 3)))->GetName() == "else"
    && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 4)))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 5))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 6)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NamedNode>(*std::next(i, 7)) && (std::static_pointer_cast<ast::NamedNode>(*std::next(i, 7)))->GetName() == "then"
    && util::instance_of<ast::SymbolNode>(*std::next(i, 8)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 8)))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 9))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 10)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 10)))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NamedNode>(*std::next(i, 11)) && (std::static_pointer_cast<ast::NamedNode>(*std::next(i, 11)))->GetName() == "if") {
    // Sc(i) -> if ( O ) then { S+ } else { S+ }
    // Currently a named node instead of identifier node
    stack->pop_back();
    std::shared_ptr<ast::StatementListNode> s1 = std::static_pointer_cast<ast::StatementListNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::StatementListNode> s2 = std::static_pointer_cast<ast::StatementListNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::ConditionalExpressionNode> e = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::IfNode> f = std::make_shared<ast::IfNode>(e->GetOperand(), s2, s1);
    context->GetStatementCounter();
    stack->push_back(f);
    return true;
  } else {
    return Subparser::Parse(context);
  }
}
}
