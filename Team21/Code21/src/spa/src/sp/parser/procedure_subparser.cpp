#include "sp/ast/astlib.h"
#include "procedure_subparser.h"
#include "util/instance_of.h"
#include "util/is_symbol_node_value.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
auto ProcedureSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // procedure: 'procedure' proc_name '{' stmtLst '}'
  if (stack->size() >= 5
    && IsSymbolNodeValue(*iter, "}")
    && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))
    && IsSymbolNodeValue(*std::next(iter, 2), "{")
    && util::instance_of<ast::IdentifierNode>(*std::next(iter, 3))
    && util::instance_of<ast::IdentifierNode>(*std::next(iter, 4)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 4)))->GetValue() == "procedure") {
    // Pops right brace symbol node
    stack->pop_back();
    // References statement list node
    std::shared_ptr<ast::StatementListNode> lis =
        std::static_pointer_cast<ast::StatementListNode>(stack->back());
    // Pops statement list node
    stack->pop_back();
    // Pops left brace symbol node
    stack->pop_back();
    // References identifier node for procedure name
    std::shared_ptr<ast::IdentifierNode> nam =
        std::static_pointer_cast<ast::IdentifierNode>(stack->back());
    // Pops name node
    stack->pop_back();
    // Pops 'procedure' identifier node
    stack->pop_back();
    // Creates procedure node
    std::shared_ptr<ast::ProcedureNode> pro =
        std::make_shared<ast::ProcedureNode>(nam->GetValue(), lis);
    // Pushes procedure node to parse stack
    stack->push_back(pro);
    return true;
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
}  // namespace parser
