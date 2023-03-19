#include "sp/ast/astlib.h"
#include "if_subparser.h"
#include "util/instance_of.h"

namespace parser {
auto IfSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
  if (stack->size() >= 12
    && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NamedNode>(*std::next(iter, 3)) && (std::static_pointer_cast<ast::NameNode>(*std::next(iter, 3)))->GetName() == "else"
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 4)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 4)))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 5))
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 6)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 6)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NameNode>(*std::next(iter, 7)) && (std::static_pointer_cast<ast::NameNode>(*std::next(iter, 7)))->GetName() == "then"
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 8)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 8)))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 9))
    && util::instance_of<ast::SymbolNode>(*std::next(iter, 10)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(iter, 10)))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NameNode>(*std::next(iter, 11)) && (std::static_pointer_cast<ast::NameNode>(*std::next(iter, 11)))->GetName() == "if") {
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> lis1 =
        std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // Pops 'else' name node
    stack->pop_back();
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> lis2 =
        std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // Pops 'then' name node
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
    // Pops 'if' name node
    stack->pop_back();
    // Creates if node
    std::shared_ptr<ast::IfNode> sta =
        std::make_shared<ast::IfNode>(exp->GetOperand(), lis2, lis1);
    context->GetStatementCounter()++;
    // Pushes if node to parse stack
    stack->push_back(sta);
    return true;
  }
  return Subparser::Parse(context);
}
}  // namespace parser
