#include "sp/ast/astlib.h"
#include "while_subparser.h"
#include "util/instance_of.h"

namespace parser {
bool WhileSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
  if (stack->size() >= 7
    && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 3)))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 4))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 5)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 5)))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NameNode>(*std::next(i, 6)) && (std::static_pointer_cast<ast::NameNode>(*std::next(i, 6)))->GetName() == "while") {
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> s = std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // Pops right paren symbol node
    stack->pop_back();
    // References conditional expression node
    std::shared_ptr<ast::ConditionalExpressionNode> e = std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
    // Pops conditional expression node
    stack->pop_back();
    // Pops left paren symbol node
    stack->pop_back();
    // Pops 'while' name node
    stack->pop_back();
    // Creates while node
    std::shared_ptr<ast::WhileNode> w = std::make_shared<ast::WhileNode>(e->GetOperand(), s);
    context->GetStatementCounter()++;
    // Pushes while node to parse stack
    stack->push_back(w);
    return true;
  }
  return Subparser::Parse(context);
}
}
