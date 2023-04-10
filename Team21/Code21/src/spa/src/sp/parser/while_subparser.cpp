#include "sp/ast/astlib.h"
#include "while_subparser.h"
#include "util/instance_of.h"
#include "util/is_identifier_node_value.h"
#include "util/is_symbol_node_value.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
auto WhileSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto while_condition = stack->size() >= 7
    && IsSymbolNodeValue(*iter, "}")
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))
    && IsNextSymbolNodesValues(std::next(iter, 2), {"{", ")"})
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(iter, 4))
    && IsSymbolNodeValue(*std::next(iter, 5), "(")
    && IsIdentifierNodeValue(*std::next(iter, 6), "while");
  // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
  if (while_condition) {
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
    // Pops 'while' identifier node
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
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}  // namespace parser
