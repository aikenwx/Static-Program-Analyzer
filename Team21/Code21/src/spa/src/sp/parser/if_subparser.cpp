#include "sp/ast/astlib.h"
#include "if_subparser.h"
#include "sp/design_extractor/extractor.h"
#include "util/instance_of.h"
#include "util/is_symbol_node_value.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto IfSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
  if (stack->size() >= 12
    && IsSymbolNodeValue(*iter, "}")
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))
    && IsSymbolNodeValue(*std::next(iter, 2), "{")
    && util::instance_of<ast::IdentifierNode>(*std::next(iter, 3)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 3)))->GetValue() == "else"
    && IsSymbolNodeValue(*std::next(iter, 4), "}")
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 5))
    && IsSymbolNodeValue(*std::next(iter, 6), "{")
    && util::instance_of<ast::IdentifierNode>(*std::next(iter, 7)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 7)))->GetValue() == "then"
    && IsSymbolNodeValue(*std::next(iter, 8), ")")
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 9))
    && IsSymbolNodeValue(*std::next(iter, 10), "(")
    && util::instance_of<ast::IdentifierNode>(*std::next(iter, 11)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 11)))->GetValue() == "if") {
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> lis1 =
        std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // Pops 'else' identifier node
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
    // Pops 'then' identifier node
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
    // Pops 'if' identifier node
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
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
