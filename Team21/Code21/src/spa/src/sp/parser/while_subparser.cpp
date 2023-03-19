#include "sp/ast/astlib.h"
#include "while_subparser.h"
#include "util/instance_of.h"

namespace parser {
auto WhileSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
  if (stack->size() >= 7
    && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 3)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 3)))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 4))
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 5)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 5)))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NameNode>(*std::next(iter, 6)) && (std::static_pointer_cast<ast::NameNode>(*std::next(iter, 6)))->GetName() == "while") {
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> lis =
        std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // Pops right paren symbol node
    stack->pop_back();
    // References conditional expression node
    std::shared_ptr<ast::ConditionalExpressionNode> exp =
        std::static_pointer_cast<ast::ConditionalExpressionNode>(stack->back());
    // Pops conditional expression node
    stack->pop_back();
    // Pops left paren symbol node
    stack->pop_back();
    // Pops 'while' name node
    stack->pop_back();
    // Creates while node
    std::shared_ptr<ast::WhileNode> sta =
        std::make_shared<ast::WhileNode>(exp->GetOperand(), lis);
    context->GetStatementCounter()++;
    // Pushes while node to parse stack
    stack->push_back(sta);
    return true;
  }
  return Subparser::Parse(context);
}
}  // namespace parser
